#ifndef MINIMAP_H
#define MINIMAP_H

#include <vector>
#include "texture.h"
#include "entity.h"

class Minimap
{
    private:
        double ratio;
        Texture* texture;
        SDL_Renderer* renderer;

    public:
        Minimap();
        Minimap(double ratio, SDL_Renderer* renderer);
        Minimap(std::string decoration, SDL_Renderer* renderer);
        Minimap(double ratio, std::string decoration, SDL_Renderer* renderer);
        void Render(std::vector<Entity*> entities, Sint32 destX, Sint32 destY);
};

#endif // MINIMAP_H
