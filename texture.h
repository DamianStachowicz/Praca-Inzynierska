#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

class Texture
{
    private:
        SDL_Renderer* renderer;
        SDL_Texture*  rawTexture;
        Uint16 width;
        Uint16 height;

    public:
        std::string path;

        Texture();
        Texture(SDL_Renderer *renderer, std::string path);
        Texture(SDL_Renderer *renderer, std::string text, TTF_Font* font, SDL_Color textColor);
        void Render(Sint16 srcX, Sint16 srcY, Uint16 srcWidth, Uint16 srcHeight, Sint16 destX, Sint16 destY, Uint16 destWidth, Uint16 destHeight, double rotation, SDL_RendererFlip flip);
        void Render(Sint16 srcX, Sint16 srcY, Uint16 srcWidth, Uint16 srcHeight, Sint16 destX, Sint16 destY, double rotation, SDL_RendererFlip flip);
        void Render(Sint16 srcX, Sint16 srcY, Uint16 srcWidth, Uint16 srcHeight, Sint16 destX, Sint16 destY, double rotation);
        void Render(Sint16 srcX, Sint16 srcY, Sint16 destX, Sint16 destY, double rotation);
        Uint16 Width();
        Uint16 Height();
        SDL_Renderer* Renderer();
};

#endif // TEXTURE_H
