#include "planet.h"

Planet::Planet() : Entity()
{
    type = ENTITY_TYPE_PLANET;
}

bool Planet::Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer) {
    return Entity::Deserialize(root, renderer);
}
