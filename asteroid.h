#ifndef ASTEROID_H
#define ASTEROID_H

#include "entity.h"
#include "particle.h"

enum {
    ASTEROID_SIZE_DEFAULT,
    ASTEROID_SIZE_SMALL,
    ASTEROID_SIZE_MIDDLE,
    ASTEROID_SIZE_BIG
};

class Asteroid : public Entity
{
    public:
        Uint8 size;

        Asteroid();
        void ReactToBlast(vector2d force);
        void ParticleCollide(Entity* particle);
        bool Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root);
        bool Deserialize(tinyxml2::XMLNode *root, SDL_Renderer* renderer);
};

#endif // ASTEROID_H
