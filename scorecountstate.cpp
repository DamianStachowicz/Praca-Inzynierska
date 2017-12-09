#include "scorecountstate.h"

ScoreCountState::ScoreCountState()
{

}

ScoreCountState::ScoreCountState(SDL_Renderer *renderer, Uint32 windowWidth, Uint32 windowHeight,
                                 Texture *background, std::function<void ()> Return2Menu) {
    this->renderer = renderer;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->background = background;
    this->EndProgram = Return2Menu;
    this->currentBtnIdx = 0;
}

void ScoreCountState::SetScore(Uint32 asteroidScore, Sint8 healthScore, Uint32 timeScore) {
    TTF_Font* font = TTF_OpenFont( "ttf/kenvector_future.ttf", 40 );
    this->asteroidScore = new Texture(renderer, "Punkty: " + std::to_string(asteroidScore),
                                      font, {0xFF, 0xFF, 0x0, 0xFF});
    this->healthScore = new Texture(renderer, "Bonus za stan statku: " + std::to_string(Uint32(healthScore)),
                                      font, {0xFF, 0xFF, 0x0, 0xFF});
    this->timeScore = new Texture(renderer, "Bonus czasowy: " + std::to_string(timeScore),
                                      font, {0xFF, 0xFF, 0x0, 0xFF});
    this->totalScore = new Texture(renderer, "W sumie: " + std::to_string(asteroidScore + healthScore + timeScore),
                                      font, {0xFF, 0xFF, 0x0, 0xFF});
}

void ScoreCountState::Render() {
    // Czyszczenie renderera przed renderowaniem nowej klatki
    SDL_RenderClear(renderer);

    // rendereowanie tła
    uint x = windowWidth / background->Width() + 1;
    uint y = windowHeight / background->Height() + 1;
    for(uint j = 0; j < y; j++) {
        for(uint i = 0; i < x; i++) {
            background->Render(0, 0, background->Width() * i, background->Height() * j, 0);
        }
    }

    // rendereowanie napisów
    asteroidScore->Render(0, 0, windowWidth / 2 - asteroidScore->Width() / 2, 60, 0);
    healthScore->Render(0, 0, windowWidth / 2 - healthScore->Width() / 2, 100, 0);
    timeScore->Render(0, 0, windowWidth / 2 - timeScore->Width() / 2, 140, 0);
    totalScore->Render(0, 0, windowWidth / 2 - totalScore->Width() / 2, 180, 0);

    // rendereowanie przycisków
    for(uint i = 0; i < buttons.size(); i++) {
        buttons[i]->Render();
    }

    // Rysowanie wyrenderowanej klatki
    SDL_RenderPresent(renderer);
}
