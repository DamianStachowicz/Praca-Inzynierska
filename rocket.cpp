#include "rocket.h"

Rocket::Rocket() : Entity()
{
    type = ENTITY_TYPE_ROCKET;
    blastRadius = 50;
    lifeSpan = 1000;
    startTime = SDL_GetTicks();
}

Rocket::Rocket(Uint32 lifeSpan) : Entity() {
    type = ENTITY_TYPE_ROCKET;
    blastRadius = 50;
    this->lifeSpan = lifeSpan;
    startTime = SDL_GetTicks();
}

void Rocket::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        if(colliding[i]->type != ENTITY_TYPE_SPACESHIP) {
            Explode();
            break;
        }
    }
    colliding.clear();
}

void Rocket::Explode() {
    /* rakieta rozpada się na cząstki o określonej długości życia,
     * które mogą zderzyć się z innymi obiektami
     */
    uint numberOfParticles = 30;
    double particleMass = mass / numberOfParticles;
    double angle = 2*M_PI / numberOfParticles;
    double initVelocity = 0.5;
    for(uint i = 0; i < numberOfParticles; i++) {
        vector2d unitVector = vector2d(cos(i*angle), sin(i*angle));
        Particle* tmp = new Particle();
        tmp->Load(texture->Renderer(), "gfx/particle.png", 34, 16, 32, location + unitVector / 10, particleMass);
        tmp->SetInitialVelocity(velocity + unitVector * initVelocity);
        tmp->rotation = 360 / numberOfParticles * i;
        entities.push_back(tmp);
    }
    alive = false;
}

void Rocket::Loop()
{
    Entity::Loop();
    if(deathAnimationStarted != 0 && SDL_GetTicks() - deathAnimationStarted > 100) {
        alive = false;
    }
    // jeżeli upłynął wyznaczony czas, rakieta eksploduje
    if(SDL_GetTicks() > startTime + lifeSpan) {
        Explode();
    }
}

bool Rocket::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji obiektu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Rocket><r>" << r;
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
    file << "</mass><blastRadius>" << blastRadius;
    file << "</blastRadius><startTime>" << startTime;
    file << "</startTime><lifeSpan>" << lifeSpan;
    file << "</lifeSpan>";
    animation.Serialize(file);
    file << "</Rocket>";
    return true;
}

bool Rocket::Deserialize(std::ifstream &file, SDL_Renderer* renderer) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji obiektu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }
    std::string tmpStr;
    XMLhelper::SkipTag(file, "<Rocket>");
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
    blastRadius = std::stod(XMLhelper::GetValue(file, "<blastRadius>"));
    startTime = std::stoul(XMLhelper::GetValue(file, "<statTime>"));
    lifeSpan = std::stoul(XMLhelper::GetValue(file, "<lifeSpan>"));
    XMLhelper::SkipTag(file, "<Animation>");
    animation.Deserialize(file);
    XMLhelper::SkipTag(file, "</Animation>");
    XMLhelper::SkipTag(file, "</Rocket>");
    return true;
}
