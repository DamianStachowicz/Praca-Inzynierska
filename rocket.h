#ifndef ROCKET_H
#define ROCKET_H

#include <SDL2/SDL.h>
#include "entity.h"
#include "particle.h"

class Rocket : public Entity
{
    private:
        double blastRadius;
        Uint32 startTime;
        Uint32 lifeSpan;    // w milisekundach

    public:
        Rocket();
        Rocket(Uint32 lifeSpan);
        void Collide();
        void Explode();
        void Loop();
        bool Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer);
};

#endif // ROCKET_H
