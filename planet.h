#ifndef PLANET_H
#define PLANET_H

#include "entity.h"

class Planet : public Entity
{
    public:
        Planet();
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file, SDL_Renderer* renderer);
};

#endif // PLANET_H
