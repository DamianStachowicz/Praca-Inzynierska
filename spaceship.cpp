#include "spaceship.h"

SpaceShip::SpaceShip() : Entity()
{
    type = ENTITY_TYPE_SPACESHIP;
    health = 100;
    jet = 1;
    rocket = NULL;
}

void SpaceShip::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        Entity *ent = colliding[i];
        switch (ent->type) {
            //case ENTITY_TYPE_PLANET: Bounce(ent); DecreaseHealth(20); break;
            case ENTITY_TYPE_ROCKET: continue;
            case ENTITY_TYPE_SELLING_POINT: break;
            case ENTITY_TYPE_PARTICLE: continue;
            default: Bounce(ent); DecreaseHealth(10); break;
        }
        double sum = r + ent->r;
        vector2d direction = (location - ent->location).Direction();
        location = ent->location + direction * sum;
    }
    colliding.clear();
}

void SpaceShip::DecreaseHealth(double amount) {
    health -= amount;
    state = ( 100 - health ) / 20;
}

void SpaceShip::UpdateForce() {
    if(stop) {
        velocity = velocity * 0.995;
    }
    Entity::UpdateForce();
    // chwilowo potrzebna jest rotacja w radianach, a nie w stopniach
    double rotation = this->rotation * M_PI / 180.0;
    vector2d direction = vector2d(cos(rotation), sin(rotation));
    if(advance) {
        force = force + ( direction * jet );
    }
    double camX = Camera::camera.windowWidth  / 2 - this->location.x;
    double camY = Camera::camera.windowHeight / 2 - this->location.y;
    Camera::camera.location = vector2d(camX, camY);
}

void SpaceShip::Rotate(double angle) {
    // Kąt podany jest w radianach, a rotacja jest w stopniach
    rotation = angle * 180 / M_PI;
}

void SpaceShip::Shoot(SDL_Renderer* renderer, vector2d initVelocity) {
    rocket = new Rocket();
    entities.push_back(rocket);
    vector2d location = this->location + vector2d(cos(rotation), sin(rotation));
    rocket->Load(renderer, "gfx/rocket.png", 16, 2, 16, location, 10000, vector2d(17, 0), 8);
    rocket->SetInitialVelocity(velocity + initVelocity);
    rocket->rotation = this->rotation;
}

Sint8 SpaceShip::Health() {
    return health;
}

bool SpaceShip::Serialize(std::ofstream &file){
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji obiektu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<SpaceShip><r>" << r;
    file << "</r><velocity>" << velocity.x << ":" << velocity.y;
    file << "</velocity><acceleration>" << acceleration.x << ":" << acceleration.y;
    file << "</acceleration><force>" << force.x << ":" << force.y;
    file << "</force><texture>" << texture->path;
    file << "</texture><deathAnimationStarted>" << deathAnimationStarted;
    file << "</deathAnimationStarted><alive>" << alive;
    file << "</alive><location>" << location.x << ":" << location.y;
    file << "</location><rotation>" << rotation;
    file << "</rotation><type>" << (Uint32)type;
    file << "</type><mass>" << mass;
    file << "</mass><collisionCenter>" << collisionCenter.x << ":" << collisionCenter.y;
    file << "</collisionCenter>";
    animation.Serialize(file);
    file << "<health>" << (Uint32)health;
    file << "</health><jet>" << jet;
    file << "</jet><advance>" << advance;
    file << "</advance><stop>" << stop;
    file << "</stop></SpaceShip>";
    return true;
}

bool SpaceShip::Deserialize(std::ifstream &file, SDL_Renderer* renderer) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji obiektu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }

    std::string tmpStr;
    XMLhelper::SkipTag(file, "<SpaceShip>");
    r = std::stod(XMLhelper::GetValue(file, "<r>"));
    tmpStr = XMLhelper::GetValue(file, "<velocity>");
    velocity = vector2d(tmpStr);
    tmpStr = XMLhelper::GetValue(file, "<acceleration>");
    acceleration = vector2d(tmpStr);
    tmpStr = XMLhelper::GetValue(file, "<force>");
    force = vector2d(tmpStr);
    texture = new Texture(renderer, XMLhelper::GetValue(file, "<texture>"));
    deathAnimationStarted = stoi(XMLhelper::GetValue(file, "<deathAnimationStarted>"));
    if(XMLhelper::GetValue(file, "<alive>") == "1") {
        alive = true;
    } else {
        alive = false;
    }
    tmpStr = XMLhelper::GetValue(file, "<location>");
    location = vector2d(tmpStr);
    rotation = std::stod(XMLhelper::GetValue(file, "<rotation>"));
    type = (Uint8)std::stoi(XMLhelper::GetValue(file, "<type>"));
    mass = std::stod(XMLhelper::GetValue(file, "<mass>"));
    collisionCenter = XMLhelper::GetValue(file, "<collisionCenter>");
    XMLhelper::SkipTag(file, "<Animation>");
    animation.Deserialize(file);
    XMLhelper::SkipTag(file, "</Animation>");
    health = (Sint8)std::stoi(XMLhelper::GetValue(file, "<health>"));
    jet = std::stod(XMLhelper::GetValue(file, "<jet>"));
    if(XMLhelper::GetValue(file, "<advance>") == "true") {
        advance = true;
    } else {
        advance = false;
    }
    if(XMLhelper::GetValue(file, "<stop>") == "true") {
        stop = true;
    } else {
        stop = false;
    }
    XMLhelper::SkipTag(file, "</SpaceShip>");
    return true;
}
