#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"

typedef void (*OnClickFunction)(void);

class Button
{
    private:
        SDL_Renderer* renderer;
        Texture*      defaultBg;
        Texture*      selectedBg;
        Texture*      textTexture;
        TTF_Font*     font;

    public:
        std::string   text;
        bool          selected;
        int x;
        int y;

        Button(SDL_Renderer* renderer, Texture* defaultBg, Texture* selectedBg,
               TTF_Font* font, std::string text, SDL_Color color, int x, int y);
        OnClickFunction OnClick;
        void Render();
        void Switch();
};

#endif // BUTTON_H
