#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "texture.h"

class State
{
    protected:
        const Uint8*  keyStates;
        TTF_Font*     font;
        Uint32        windowWidth;
        Uint32        windowHeight;

    public:
        State();

        virtual void Init();
        virtual void Run();
        virtual void Loop();   // główna pętla gry
        virtual void Render(); // funkcja rysująca obiekty na ekranie
        virtual void HandleEvent(SDL_Event* event); // obsługa zdarzeń
        virtual void HandleKeyDown(); // obsługa wciśnięcia klawisza na klawiaturze
        virtual void HandleKeyUp(); // obsługa puszczenia klawisza na klawiaturze
        virtual void HandleMouseMotion(SDL_MouseMotionEvent* event); // obsługa zdarzenia poruszenia kursorem
        virtual void HandleMouseButtonDown(SDL_MouseButtonEvent* event); // obsługa wciśnięcia przycisku myszy
        virtual void HandleMouseWheelScroll(SDL_MouseWheelEvent* event); // obsługa przesunięcia kułeczka myszy
};

#endif // STATE_H
