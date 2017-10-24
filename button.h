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
        Texture* defaultT;
        Texture* hoverT;

    public:
        int x;
        int y;

        Button();
        Button(Texture* defaultT, Texture* hoverT, int x, int y, OnClickFunction OnClick);
        OnClickFunction OnClick;
        bool IsHovered();
        void Render();
};

#endif // BUTTON_H
