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
        bool Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer);
};

#endif // SELLINGPOINT_H
