#include "creditsstate.h"

CreditsState::CreditsState()
{

}

CreditsState::CreditsState(SDL_Renderer *renderer, Uint32 windowWidth, Uint32 windowHeight,
                         Texture *background, std::function<void ()> Return2Menu) {
    this->renderer = renderer;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->background = background;
    this->Return2Menu = Return2Menu;
    TTF_Font* font = TTF_OpenFont( "ttf/kenvector_future.ttf", 40 );
    credit1 = new Texture(renderer, "Autor: Damian Stachowicz",
                          font, {0xFF, 0xFF, 0x0, 0xFF});
    credit2 = new Texture(renderer, "Grafika: Kenney - www.kenney.nl",
                          font, {0xFF, 0xFF, 0x0, 0xFF});
    credit3 = new Texture(renderer, "Wykorzystano biblioteki SDL2 i tinyxml2",
                          font, {0xFF, 0xFF, 0x0, 0xFF});
}

void CreditsState::Render() {
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
    credit1->Render(0, 0, windowWidth / 2 - credit1->Width() / 2, 60, 0);
    credit2->Render(0, 0, windowWidth / 2 - credit2->Width() / 2, 100, 0);
    credit3->Render(0, 0, windowWidth / 2 - credit3->Width() / 2, 140, 0);

    // Rysowanie wyrenderowanej klatki
    SDL_RenderPresent(renderer);
}

void CreditsState::HandleEvent(SDL_Event *event) {
    keyStates = SDL_GetKeyboardState( NULL );
    switch( event->type )
    {
        case SDL_KEYDOWN: HandleKeyDown(); break;
        case SDL_KEYUP:   HandleKeyUp(); break;
        case SDL_WINDOWEVENT:
        {
            switch(event->window.event)
            {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    windowWidth = event->window.data1;
                    windowHeight = event->window.data2;
                    break;
                }
            }
            break;
        }
    }
}

void CreditsState::HandleKeyDown() {
    if( keyStates[ SDL_SCANCODE_ESCAPE ] ) {
        Return2Menu();
    }
}
