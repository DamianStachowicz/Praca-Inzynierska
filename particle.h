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
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file, SDL_Renderer* renderer);
};

#endif // PARTICLE_H
