#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <SDL2/SDL.h>
#include <vector>
#include "state.h"
#include "button.h"

class MenuState : public State
{
    protected:
        std::vector<Button*> buttons;
        Uint16 currentBtnIdx;
        Texture* background;
        std::function<void()> EndProgram;

    public:
        MenuState();
        MenuState(SDL_Renderer* renderer, Uint32 windowWidth, Uint32 windowHeight, Texture* background, std::function<void()> EndProgram);

        void AddButton(Button* btn);
        void ButtonUp();
        void ButtonDown();
        void Loop();   // główna pętla gry
        void Render(); // funkcja rysująca obiekty na ekranie
        void HandleEvent(SDL_Event* event); // obsługa zdarzeń
        void HandleKeyDown(); // obsługa wciśnięcia klawisza na klawiaturze
        void HandleKeyUp(); // obsługa puszczenia klawisza na klawiaturze
};

#endif // MENUSTATE_H
