#include "minimap.h"

Minimap::Minimap()
{
}

Minimap::Minimap(double ratio, std::__cxx11::string decoration, SDL_Renderer* renderer) {
    this->ratio = ratio;
    texture = new Texture(renderer, decoration);
}

Minimap::Minimap(double ratio, SDL_Renderer* renderer) {
    Minimap(ratio, "gfx/minimap.png", renderer);
}

Minimap::Minimap(std::__cxx11::string decoration, SDL_Renderer* renderer) {
    Minimap(0.001, decoration, renderer);
}

void Minimap::Render(std::vector<Entity *> entities, Sint32 destX, Sint32 destY) {
    for(uint i = 0; i < entities.size(); i++) {

    }
    texture->Render(0, 0, destX, destY, 0);
}
