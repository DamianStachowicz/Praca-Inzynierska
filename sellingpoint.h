#ifndef SELLINGPOINT_H
#define SELLINGPOINT_H

#include "entity.h"

class SellingPoint : public Entity
{
    private:
        Uint32* score;

    public:
        SellingPoint(Uint32* score);
        void UpdateForce();
        void Collide();
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file, SDL_Renderer* renderer);
};

#endif // SELLINGPOINT_H
