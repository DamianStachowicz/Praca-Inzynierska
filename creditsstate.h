#ifndef CREDITSSATE_H
#define CREDITSSATE_H

#include "state.h"

class CreditsState : public State
{
    private:
        Texture* background;
        std::function<void()> Return2Menu;
        Texture* credit1;
        Texture* credit2;
        Texture* credit3;

    public:
        CreditsState();
        CreditsState(SDL_Renderer* renderer, Uint32 windowWidth, Uint32 windowHeight,
                    Texture* background, std::function<void()> Return2Menu);
        void Render(); // funkcja rysująca obiekty na ekranie
        void HandleEvent(SDL_Event* event);
        void HandleKeyDown();
};

#endif // CREDITSSATE_H
