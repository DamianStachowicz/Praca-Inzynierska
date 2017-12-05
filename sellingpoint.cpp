#include "sellingpoint.h"

SellingPoint::SellingPoint(Uint32* score)
{
    this->score = score;
    type = ENTITY_TYPE_SELLING_POINT;
}

void SellingPoint::UpdateForce() {

}

void SellingPoint::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        if(colliding[i]->type == ENTITY_TYPE_ASTEROID) {
            *score += colliding[i]->mass;
        }
    }
    colliding.clear();
}

bool SellingPoint::Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer) {
    return Entity::Deserialize(root, renderer);
}
