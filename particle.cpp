#include "particle.h"

Particle::Particle()
{
    type = ENTITY_TYPE_PARTICLE;
    timeOfDeath = SDL_GetTicks() + 500; // w milisekundach
}

void Particle::Loop() {
    Entity::Loop();
    if(SDL_GetTicks() > timeOfDeath) {
        alive = false;
    }
}

void Particle::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        if(colliding[i]->type != ENTITY_TYPE_PARTICLE && colliding[i]->Alive()) {
            alive = false;
            break;
        }
    }
    colliding.clear();
}

bool Particle::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji obiektu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Particle><r>" << r;
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
    file << "<timeOfDeath>" << timeOfDeath;
    file << "</timeOfDeath></Particle>";
    return true;
}

bool Particle::Deserialize(std::ifstream &file, SDL_Renderer* renderer) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji obiektu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }
    std::string tmpStr;
    XMLhelper::SkipTag(file, "<Particle>");
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
    timeOfDeath = std::stoi(XMLhelper::GetValue(file, "<timeOfDeath>"));
    XMLhelper::SkipTag(file, "</Particle>");
    return true;
}
