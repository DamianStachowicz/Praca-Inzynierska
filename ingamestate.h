#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "state.h"
#include "texture.h"
#include "spaceship.h"

class InGameState : public State
{
    private:
        SDL_Renderer* renderer;
        Texture*      scoreTexture;
        SDL_Color     scoreColor;
        Texture*      scoreBg;
        Texture*      timerTexture;
        SDL_Color     timerColor;
        SpaceShip*    player;
        Uint32*       score;
        Uint32        asteroidsMass;
        bool          debug;

    public:
        std::string   filename;

        InGameState();
        InGameState(SDL_Renderer* renderer, Uint32 windowWidth, Uint32 windowHeight);
        void Init(SpaceShip *player, Uint32* score, std::function<void()> ChangeState);
        void Loop();   // główna pętla gry
        void Render(); // funkcja rysująca obiekty na ekranie
        void HandleEvent(SDL_Event* event); // obsługa zdarzeń
        void HandleKeyDown(); // obsługa wciśnięcia klawisza na klawiaturze
        void HandleKeyUp(); // obsługa puszczenia klawisza na klawiaturze
        void HandleMouseMotion(SDL_MouseMotionEvent* event); // obsługa zdarzenia poruszenia kursorem
        void HandleMouseButtonDown(SDL_MouseButtonEvent* event); // obsługa wciśnięcia przycisku myszy
        void HandleMouseWheelScroll(SDL_MouseWheelEvent* event); // obsługa przesunięcia kułeczka myszy
        void SetPlayer(SpaceShip* player);
        void SetAsteroidsMass(Uint32 mass);
};

#endif // INGAMESTATE_H
