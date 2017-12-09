#ifndef SCORECOUNTSTATE_H
#define SCORECOUNTSTATE_H

#include <menustate.h>

class ScoreCountState : public MenuState
{
    private:
        Texture* asteroidScore;
        Texture* healthScore;
        Texture* timeScore;
        Texture* totalScore;

    public:
        ScoreCountState();
        ScoreCountState(SDL_Renderer* renderer, Uint32 windowWidth, Uint32 windowHeight,
                        Texture* background, std::function<void()> Return2Menu);
        void SetScore(Uint32 asteroidScore, Sint8 healthScore, Uint32 timeScore);
        void Render(); // funkcja rysująca obiekty na ekranie
};

#endif // SCORECOUNTSTATE_H
