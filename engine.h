/** @file engine.h
 *  @author Damian Stachowicz
 *  @brief Plik nagłówkowy silnika gry
 */
#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include "texture.h"
#include "entity.h"
#include "timer.h"
#include "spaceship.h"
#include "planet.h"
#include "rocket.h"
#include "asteroidbig.h"
#include "sellingpoint.h"
#include "ingamestate.h"

/** @class Engine
 *  @brief Silnik gry
 */
class Engine
{
    private:
        SDL_Window*   window;
        SDL_Renderer* renderer;
        Uint32        windowWidth;
        Uint32        windowHeight;
        const Uint8*  keyStates;
        SpaceShip*    player;
        TTF_Font*     font;
        Texture*      scoreTexture;
        SDL_Color     scoreColor;
        Texture*      scoreBg;
        Texture*      timerTexture;
        SDL_Color     timerColor;
        InGameState   inGameState;
        State*        currentState;

    public:
        Uint32 score;

        Engine();
        ~Engine();
        bool Init(); // funkcja inicjalizująca silnik
        void Run();  // funkcja uruchamiająca grę
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file);
};

#endif // ENGINE_H
