#ifndef PARTICLE_H
#define PARTICLE_H

#include "entity.h"

class Particle : public Entity
{
    private:
        Uint32 timeOfDeath;

    public:
        Particle();
        void Loop();
        void Collide();
        bool Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root);
        bool Deserialize(tinyxml2::XMLElement* root, SDL_Renderer* renderer);
};

#endif // PARTICLE_H
