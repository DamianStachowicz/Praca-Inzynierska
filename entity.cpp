#include "entity.h"
#include "planet.h"
#include "asteroid.h"
#include "sellingpoint.h"

Timer Entity::timer = Timer(60, 1);
std::vector<Entity*> Entity::entities = std::vector<Entity*>();
Level Entity::level = Level(800);
// stała grawitacyjna
// double G = 0.000000000066742;
double G = 0.0000000066742;

Entity::Entity()
{
    location = vector2d();
    rotation = 0;
    r = 0;
    velocity = vector2d();
    acceleration = vector2d();
    force = vector2d();
    mass = 0;
    texture = NULL;
    colliding = std::vector<Entity*>();
    type = ENTITY_TYPE_UNDEFINED;
    deathAnimationStarted = 0;
    alive = true;
    animation = Animation();
    maxSpeed = 2;
    state = 0;
}

Entity::~Entity() {

}

bool Entity::CheckCollision(Entity* ent) {
    // wektor odległości między środkami encji
    vector2d v = (this->location + this->CollisionCenter()) - (ent->location + ent->CollisionCenter());
    // suma promieni
    double radia = this->r + ent->r;
    /* kolizja nastąpiła, jeśli odległość między
     * środkami jest mniejsza niż suma promieni
     */
    return (v.Length() < radia);
}

void Entity::Load(SDL_Renderer *renderer, std::__cxx11::string texturePath, Sint16 frameHeight, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass) {
    texture = new Texture(renderer, texturePath);
    if(texture->Width() > frameHeight) {
        r = texture->Width() / 2;
    } else {
        r = frameHeight / 2;
    }
    animation = Animation(numberOfFrames, framesPerSecond, texture->Width(), frameHeight);
    location = initLocation;
    this->mass = mass;
    this->collisionCenter = vector2d(0, 0);
}

void Entity::Load(SDL_Renderer *renderer, std::__cxx11::string texturePath, Sint16 frameHeight, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass, vector2d collisionCenter, double r) {
    texture = new Texture(renderer, texturePath);
    animation = Animation(numberOfFrames, framesPerSecond, texture->Width(), frameHeight);
    location = initLocation;
    this->mass = mass;
    this->collisionCenter = collisionCenter;
    this->r = r;
}

void Entity::Load(SDL_Renderer *renderer, std::__cxx11::string texturePath, Sint16 frameHeight, Sint16 frameWidth,
                  Uint8 numberOfFrames, Uint8 framesPerSecond, vector2d initLocation, double mass,
                  vector2d collisionCenter, double r) {
    texture = new Texture(renderer, texturePath);
    animation = Animation(numberOfFrames, framesPerSecond, frameWidth, frameHeight);
    location = initLocation;
    this->mass = mass;
    this->collisionCenter = collisionCenter;
    this->r = r;
}

void Entity::SetInitialVelocity(vector2d v) {
    velocity = v;
}

void Entity::Render(double xOffset, double yOffset) {
    double halfWidth = animation.Width() / 2;
    double halfHeight = animation.Height() / 2;
    Uint32 windowWidth = Camera::camera.windowWidth;
    Uint32 windowHeight = Camera::camera.windowHeight;
    // Położenie i rozmiar muszą zostać dostosowane do położenia kamery (trójwymiarowego)
    double camX = Camera::camera.location.x;
    double camY = Camera::camera.location.y;
    double zoom = Camera::camera.zoom;
    texture->Render(state * animation.Width(),
                    animation.CurrentFrame() * animation.Height(),     // srcX, srcY
                    animation.Width(), animation.Height(),                // srcWidth, srcHeight
                    (location.x - halfWidth  - camX + xOffset) * zoom + windowWidth / 2,  // destX
                    (location.y - halfHeight - camY + yOffset) * zoom + windowHeight / 2, // destY
                    animation.Width() * zoom, animation.Height() * zoom,  // destWidth, destHeight
                    rotation, SDL_FLIP_NONE);
}

void Entity::Render() {
    Render(0, 0);
}

vector2d Entity::AttractTo(Entity* ent) {
    vector2d dist = ent->location - this->location;
    if(dist.Length() != 0) {
        return dist.Direction() * ( G * this->mass * ent->mass / (dist.Length() * dist.Length()) );
    } else {
        return vector2d();
    }
}

void Entity::UpdateAcceleration() {
    acceleration = force.Direction() * (force.Length() / mass);
}

void Entity::Move() {
    Uint8 dt = timer.GetDt();
    acceleration = force / mass;
    velocity = velocity + acceleration * dt;
    // ograniczenie prędkości
    if(velocity.Length() > maxSpeed) {
        double ratio = velocity.Length() / maxSpeed;
        velocity = velocity / ratio;
    }
    vector2d shift = velocity * dt;
    location = location + shift;
    if( location.x > level.r || location.x < -level.r  ) {
        location.x = -location.x;
    }
    if( location.y > level.r || location.y < -level.r ) {
        location.y = -location.y;
    }
}

// wykrywanie kolizji
void Entity::CheckCollisions() {
    // sprawdzanie wszystkich encji
    for(uint i = 0; i < entities.size(); i++) {
        // pominięcie samej siebie
        if(entities[i] != this && CheckCollision(entities[i])) {
            // jeżeli wykryto kolizję to encja jest wrzucana do kolejki do obsłużenia zderzenia
            colliding.push_back(entities[i]);
        }
    }
}

void Entity::UpdateForce() {
    vector2d force = vector2d(0, 0);
    for(uint i = 0; i < entities.size(); i++) {
        if(entities[i] != this) {
            force = force + AttractTo(entities[i]);
        }
    }
    this->force = force;
}

void Entity::Collide() {
}

bool Entity::Alive() {
    return alive;
}

bool Entity::DeathStarted() {
    return !(deathAnimationStarted == 0);
}

void Entity::Loop() {
    UpdateForce();
    Move();
}

vector2d Entity::Momentum() {
    return velocity * mass;
}

void Entity::Bounce(Entity *ent) {
    double massFactor = 2 * ent->mass / (mass + ent->mass);
    vector2d loc = (location - ent->location);
    double a = (velocity - ent->velocity) * loc;
    double b = loc.Length() * loc.Length();
    velocity = velocity - loc * (massFactor * a / b);
}

vector2d Entity::GetMinimapLocation(double ratio) {
    return location * ratio;
}

double Entity::GetMinimapR(double ratio) {
    return r * ratio;
}

void Entity::RenderCopy(int position) {
    switch(position) {
        case LEVEL_COPY_NONE: break;
        case LEVEL_COPY_DEFAULT: Render(0, 0); break;
        case LEVEL_COPY_MIDDLE_BOTTOM: Render(0, 2 * level.r); break;
        case LEVEL_COPY_LEFT_BOTTOM: Render(-2 * level.r, 2 * level.r); break;
        case LEVEL_COPY_LEFT_MIDDLE: Render(-2 * level.r, 0); break;
        case LEVEL_COPY_LEFT_TOP: Render(-2 * level.r, -2 * level.r); break;
        case LEVEL_COPY_MIDDLE_TOP: Render(0, -2 * level.r);  break;
        case LEVEL_COPY_RIGHT_TOP: Render(2 * level.r, -2 * level.r);  break;
        case LEVEL_COPY_RIGHT_MIDDLE: Render(2 * level.r, 0);  break;
        case LEVEL_COPY_RIGHT_BOTTOM: Render(2 * level.r, 2 * level.r);  break;
    }
}

bool Entity::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* subelement = xmlDoc->NewElement("r");
    subelement->SetText(r);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("velocity");
    tinyxml2::XMLElement* subsubelement = xmlDoc->NewElement("x");
    subsubelement->SetText(velocity.x);
    subelement->InsertEndChild(subsubelement);
    subsubelement = xmlDoc->NewElement("y");
    subsubelement->SetText(velocity.y);
    subelement->InsertEndChild(subsubelement);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("acceleration");
    subsubelement = xmlDoc->NewElement("x");
    subsubelement->SetText(acceleration.x);
    subelement->InsertEndChild(subsubelement);
    subsubelement = xmlDoc->NewElement("y");
    subsubelement->SetText(acceleration.y);
    subelement->InsertEndChild(subsubelement);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("force");
    subsubelement = xmlDoc->NewElement("x");
    subsubelement->SetText(force.x);
    subelement->InsertEndChild(subsubelement);
    subsubelement = xmlDoc->NewElement("y");
    subsubelement->SetText(force.y);
    subelement->InsertEndChild(subsubelement);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("texture");
    subelement->SetText(texture->path.c_str());
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("deathAnimationStarted");
    subelement->SetText(deathAnimationStarted);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("alive");
    subelement->SetText(alive);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("location");
    subsubelement = xmlDoc->NewElement("x");
    subsubelement->SetText(location.x);
    subelement->InsertEndChild(subsubelement);
    subsubelement = xmlDoc->NewElement("y");
    subsubelement->SetText(location.y);
    subelement->InsertEndChild(subsubelement);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("rotation");
    subelement->SetText(rotation);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("type");
    subelement->SetText(type);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("mass");
    subelement->SetText(mass);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("collisionCenter");
    subsubelement = xmlDoc->NewElement("x");
    subsubelement->SetText(collisionCenter.x);
    subelement->InsertEndChild(subsubelement);
    subsubelement = xmlDoc->NewElement("y");
    subsubelement->SetText(collisionCenter.y);
    subelement->InsertEndChild(subsubelement);
    root->InsertEndChild(subelement);

    subelement = xmlDoc->NewElement("animation");
    animation.Serialize(xmlDoc, subelement);
    root->InsertEndChild(subelement);

    return true;
}

bool Entity::Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer) {
    tinyxml2::XMLElement* element = root->FirstChildElement("r");
    element->QueryDoubleText(&r);

    element = root->FirstChildElement("velocity");
    tinyxml2::XMLElement* subelement = element->FirstChildElement("x");
    subelement->QueryDoubleText(&velocity.x);
    subelement = element->FirstChildElement("y");
    subelement->QueryDoubleText(&velocity.y);

    element = root->FirstChildElement("acceleration");
    subelement = element->FirstChildElement("x");
    subelement->QueryDoubleText(&acceleration.x);
    subelement = element->FirstChildElement("y");
    subelement->QueryDoubleText(&acceleration.y);

    element = root->FirstChildElement("force");
    subelement = element->FirstChildElement("x");
    subelement->QueryDoubleText(&force.x);
    subelement = element->FirstChildElement("y");
    subelement->QueryDoubleText(&force.y);

    element = root->FirstChildElement("texture");
    std::string path = element->GetText();
    texture = new Texture(renderer, path);

    element = root->FirstChildElement("deathAnimationStarted");
    element->QueryUnsignedText(&deathAnimationStarted);

    element = root->FirstChildElement("alive");
    element->QueryBoolText(&alive);

    element = root->FirstChildElement("location");
    subelement = element->FirstChildElement("x");
    subelement->QueryDoubleText(&location.x);
    subelement = element->FirstChildElement("y");
    subelement->QueryDoubleText(&location.y);

    element = root->FirstChildElement("rotation");
    element->QueryDoubleText(&rotation);

    Uint32 tmp;
    element = root->FirstChildElement("type");
    element->QueryUnsignedText(&tmp);
    type = tmp;

    element = root->FirstChildElement("mass");
    element->QueryDoubleText(&mass);

    element = root->FirstChildElement("collisionCenter");
    subelement = element->FirstChildElement("x");
    subelement->QueryDoubleText(&collisionCenter.x);
    subelement = element->FirstChildElement("y");
    subelement->QueryDoubleText(&collisionCenter.y);

    element = root->FirstChildElement("animation");
    animation.Deserialize(element);

    return true;
}

vector2d Entity::CollisionCenter() {
    double angle = rotation * M_PI / 180;
    return vector2d(collisionCenter.x * cos(angle) - collisionCenter.y * sin(angle),
                    collisionCenter.x * sin(angle) + collisionCenter.y * cos(angle));
}

Uint8 Entity::VisibleCopy() {
    double leftEdge   = Camera::camera.LeftEdge(level.r);
    double rightEdge  = Camera::camera.RightEdge(level.r);
    double topEdge    = Camera::camera.TopEdge(level.r);
    double bottomEdge = Camera::camera.BottomEdge(level.r);

    if(leftEdge < rightEdge) {
        if(topEdge < bottomEdge) {
            // kamera nie podzielona
            if(location.x + animation.Width() < leftEdge) {
                // jeżeli na lewo od lewej krawędzi kamery to encja nie jest widoczna
                return LEVEL_COPY_NONE;
            }
            if(location.x - animation.Width() > rightEdge) {
                // jeżeli na prawo od lewej krawędzi kamery to encja nie jest widoczna
                return LEVEL_COPY_NONE;
            }
            if(location.y + animation.Height() < topEdge) {
                // jeżeli powyżej górnej krawędzi to encja nie jest widoczna
                return LEVEL_COPY_NONE;
            }
            if(location.y - animation.Height() > bottomEdge) {
                // jeżeli poniżej górnej krawędzi to encja nie jest widoczna
                return LEVEL_COPY_NONE;
            }
            return LEVEL_COPY_DEFAULT;
        } else {
            // kamera podzielona w pionie
            // kopia w zależności od położenia
            if(topEdge <= level.r - Camera::camera.windowHeight / 2) {
                // środek kamery na dole
                if(location.y + animation.Height() / 2 >= topEdge) {
                    return LEVEL_COPY_DEFAULT;
                }
                if(location.y - animation.Height() / 2 <= bottomEdge) {
                    return LEVEL_COPY_MIDDLE_BOTTOM;
                }
            } else {
                // środek kamery na górze
                if(location.y + animation.Height() / 2 >= topEdge) {
                    return LEVEL_COPY_MIDDLE_TOP;
                }
                if(location.y - animation.Height() / 2 <= bottomEdge) {
                    return LEVEL_COPY_DEFAULT;
                }
            }
        }
    } else {
        if(topEdge < bottomEdge) {
            // kamera podzielona w poziomie
            // kopia w zależności od położenia
            if(leftEdge <= level.r - Camera::camera.windowWidth / 2) {
                // środek kamery po prawej
                if(location.x + animation.Width() / 2 >= leftEdge) {
                    return LEVEL_COPY_DEFAULT;
                }
                if(location.x - animation.Width() / 2 <= rightEdge) {
                    return LEVEL_COPY_RIGHT_MIDDLE;
                }
                return LEVEL_COPY_DEFAULT;
            } else {
                // środek kamery po lewej
                if(location.x + animation.Width() / 2 >= leftEdge) {
                    return LEVEL_COPY_LEFT_MIDDLE;
                }
                if(location.x - animation.Width() / 2 <= rightEdge) {
                    return LEVEL_COPY_DEFAULT;
                }
                return LEVEL_COPY_DEFAULT;
            }
        } else {
            // kamera podzielona na cztery
            // kopia w zależności od położenia
            if(rightEdge >= -level.r + Camera::camera.windowWidth / 2 &&
               bottomEdge >= -level.r + Camera::camera.windowHeight / 2) {
                // środek kamery w lewym górnym rogu
                if(location.y - animation.Height() / 2 <= bottomEdge) {
                    // górne ćwiartki
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa górna ćwiartka
                        return LEVEL_COPY_DEFAULT;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa górna ćwiartka
                        return LEVEL_COPY_LEFT_MIDDLE;
                    }
                }
                if(location.y + animation.Height() / 2 >= topEdge) {
                    // dolna ćwiartka
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa dolna ćwiartka
                        return LEVEL_COPY_MIDDLE_TOP;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa dolna ćwiartka
                        return LEVEL_COPY_LEFT_TOP;
                    }
                }
                return LEVEL_COPY_DEFAULT;
            } else if(leftEdge <= level.r - Camera::camera.windowWidth / 2 &&
                      bottomEdge >= -level.r + Camera::camera.windowHeight / 2) {
                // środek kamery w prawym górnym rogu
                if(location.y - animation.Height() / 2 <= bottomEdge) {
                    // górne ćwiartki
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa górna ćwiartka
                        return LEVEL_COPY_RIGHT_MIDDLE;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa górna ćwiartka
                        return LEVEL_COPY_DEFAULT;
                    }
                }
                if(location.y + animation.Height() / 2 >= topEdge) {
                    // dolna ćwiartka
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa dolna ćwiartka
                        return LEVEL_COPY_RIGHT_TOP;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa dolna ćwiartka
                        return LEVEL_COPY_MIDDLE_TOP;
                    }
                }
                return LEVEL_COPY_DEFAULT;
            } else if(leftEdge <= level.r - Camera::camera.windowWidth / 2 &&
                      topEdge <= level.r - Camera::camera.windowHeight / 2) {
                // środek kamery w prawym dolnym rogu
                if(location.y - animation.Height() / 2 <= bottomEdge) {
                    // górne ćwiartki
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa górna ćwiartka
                        return LEVEL_COPY_RIGHT_BOTTOM;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa górna ćwiartka
                        return LEVEL_COPY_MIDDLE_BOTTOM;
                    }
                }
                if(location.y + animation.Height() / 2 >= topEdge) {
                    // dolna ćwiartka
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa dolna ćwiartka
                        return LEVEL_COPY_RIGHT_MIDDLE;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa dolna ćwiartka
                        return LEVEL_COPY_DEFAULT;
                    }
                }
                return LEVEL_COPY_DEFAULT;
            } else {
                // środek kamery w lewym dolnym rogu
                if(location.y - animation.Height() / 2 <= bottomEdge) {
                    // górne ćwiartki
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa górna ćwiartka
                        return LEVEL_COPY_MIDDLE_BOTTOM;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa górna ćwiartka
                        return LEVEL_COPY_LEFT_BOTTOM;
                    }
                }
                if(location.y + animation.Height() / 2 >= topEdge) {
                    // dolna ćwiartka
                    if(location.x - animation.Width() <= rightEdge) {
                        // lewa dolna ćwiartka
                        return LEVEL_COPY_DEFAULT;
                    }
                    if(location.x + animation.Width() / 2 >= leftEdge) {
                        // prawa dolna ćwiartka
                        return LEVEL_COPY_LEFT_MIDDLE;
                    }
                }
                return LEVEL_COPY_DEFAULT;
            }
        }
    }
    return LEVEL_COPY_NONE;
}
