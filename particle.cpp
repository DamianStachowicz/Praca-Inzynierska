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

bool Particle::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* subelement = xmlDoc->NewElement("timeOfDeath");
    subelement->SetText(timeOfDeath);
    root->InsertEndChild(subelement);
    return Entity::Serialize(xmlDoc, root);
}

bool Particle::Deserialize(tinyxml2::XMLElement *root, SDL_Renderer *renderer) {
    tinyxml2::XMLElement* element = root->FirstChildElement("timeOfDeath");
    element->QueryUnsignedText(&timeOfDeath);
    return Entity::Deserialize(root, renderer);
}
