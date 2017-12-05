#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include <tinyxml2.h>
#include "xmlhelper.h"

// Zarządza krokiem czasowym symulacji oraz liczbą klatek na sekundę

class Timer
{
    private:
        Uint8 FPS;
        Uint8 dt; // krok czasowy symulacji [ms]
        Uint32 currentTime;
        Uint16 stepsPerFrame;

    public:
        Uint32 lastFrameTime; // czas, w którym wyświetlono poprzednią klatkę

        Timer();
        Timer(Uint8 FPS, Uint8 dt);
        Uint8 GetFPS();
        Uint8 GetDt();
        Uint16 GetStepsPerFrame();
        void NextStep();
        void UpdateLastFrameTime();
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file);
        bool Serialize(tinyxml2::XMLDocument* xmlDoc, tinyxml2::XMLNode* root);
        bool Deserialize(tinyxml2::XMLNode* root);
};

#endif // TIMER_H
