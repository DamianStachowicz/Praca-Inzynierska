#ifndef PLANET_H
#define PLANET_H

#include "entity.h"

class Planet : public Entity
{
    public:
        Planet();
        bool Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer);
};

#endif // PLANET_H
