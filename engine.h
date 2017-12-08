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
#include "menustate.h"
#include "lvlpickstate.h"
#include "creditsstate.h"

/** @class Engine
 *  @brief Silnik gry
 */
class Engine
{
    private:
        bool          quit;
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
        MenuState     menuState;
        LvlPickState  lvlPickState;
        CreditsState  creditsState;
        State*        currentState;
        Uint32        asteroidsMass;

        // przełączanie stanów
        void StartGame();
        void EndGame();
        void LoadLevel();
        void LevelPick();
        void EndProgram();
        void Return2Menu();
        void ShowCredits();

    public:
        Uint32 score;

        Engine();
        ~Engine();
        bool Init(); // funkcja inicjalizująca silnik
        void Run();  // funkcja uruchamiająca grę
        bool Serialize(tinyxml2::XMLDocument* xmlDoc, tinyxml2::XMLNode* root);
        bool Deserialize(tinyxml2::XMLNode* root);
        void Clear();
};

#endif // ENGINE_H
