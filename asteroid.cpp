#include "asteroid.h"

Asteroid::Asteroid()
{
    type = ENTITY_TYPE_ASTEROID;
    size = 0;
}

void Asteroid::ReactToBlast(vector2d force) {
    this->force = this->force + force;
}

void Asteroid::ParticleCollide(Entity *particle) {
    velocity = velocity + ( particle->Momentum() / mass );
}

bool Asteroid::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji obiektu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<size>" << (Uint32)size;
    file << "</size>";
    file << "<Asteroid><r>" << r;
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
    file << "</mass>";
    animation.Serialize(file);
    file << "</Asteroid>";
    return true;
}

bool Asteroid::Deserialize(std::ifstream &file, SDL_Renderer* renderer) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji obiektu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }

    std::string tmpStr;
    XMLhelper::SkipTag(file, "<Asteroid>");
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
    //size = std::stoi(XMLhelper::GetValue(file, "<size>"));
    XMLhelper::SkipTag(file, "<Animation>");
    animation.Deserialize(file);
    XMLhelper::SkipTag(file, "</Animation>");
    XMLhelper::SkipTag(file, "</Asteroid>");
    return true;
}