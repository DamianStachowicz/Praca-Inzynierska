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
    // velocity = velocity + ( particle->Momentum() / mass );

    double massFactor = 2 * particle->mass / (mass + particle->mass);
    vector2d loc = (location - particle->location);
    double a = (velocity - particle->velocity) * loc;
    double b = loc.Length() * loc.Length();
    velocity = velocity - loc * (massFactor * a / b);
}

bool Asteroid::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* subelement = xmlDoc->NewElement("size");
    subelement->SetText(size);
    root->InsertEndChild(subelement);
    return Entity::Serialize(xmlDoc, root);
}

bool Asteroid::Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer) {
    tinyxml2::XMLElement* element = root->FirstChildElement("size");
    Uint32 tmp;
    element->QueryUnsignedText(&tmp);
    size = tmp;
    return Entity::Deserialize(root, renderer);
}
