#include "planet.h"

Planet::Planet() : Entity()
{
    type = ENTITY_TYPE_PLANET;
}

bool Planet::Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer) {
    return Entity::Deserialize(root, renderer);
}

void Planet::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        Entity *ent = colliding[i];
        Bounce(ent);
        double sum = r + ent->r;
        vector2d direction = (location - ent->location).Direction();
        location = ent->location + direction * sum;
    }
    colliding.clear();
}
