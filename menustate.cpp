#include "menustate.h"

MenuState::MenuState() {

}

MenuState::MenuState(SDL_Renderer *renderer, Uint32 windowWidth, Uint32 windowHeight, Texture *background) {
    this->renderer = renderer;
    this->windowHeight = windowHeight;
    this->windowWidth = windowWidth;
    this->background = background;
    currentBtnIdx = 0;
}

void MenuState::AddButton(Button* btn) {
    buttons.push_back(btn);
}

void MenuState::ButtonUp() {
    buttons[currentBtnIdx]->Switch();
    if( currentBtnIdx == 0 ) {
        currentBtnIdx = buttons.size();
    }
    currentBtnIdx--;
    buttons[currentBtnIdx]->Switch();
}

void MenuState::ButtonDown() {
    buttons[currentBtnIdx]->Switch();
    currentBtnIdx++;
    currentBtnIdx = currentBtnIdx % buttons.size();
    buttons[currentBtnIdx]->Switch();
}

void MenuState::Run() {
    SDL_Event event;
    bool quit = false;

    // program pozostaje uruchomiony dopóki użytkownik nie zarząda zakończenia
    while( !quit )
    {
        // przetwarzanie kolejki zdarzeń
        while( SDL_PollEvent( &event ) != 0 )
        {
            // zamknięcie aplikacji
            if(event.type == SDL_QUIT){
                quit = true;
            } else {
                HandleEvent(&event);
            }
        }

        Loop();
        Render();
    }
}

void MenuState::HandleEvent(SDL_Event *event) {
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

void MenuState::HandleKeyDown() {
    if( keyStates[ SDL_SCANCODE_S ] || keyStates[ SDL_SCANCODE_DOWN ] ) {
        ButtonDown();
    } else if( keyStates[ SDL_SCANCODE_W ] || keyStates[ SDL_SCANCODE_UP ] ) {
        ButtonUp();
    }
}

void MenuState::HandleKeyUp() {

}

void MenuState::Loop() {

}

void MenuState::Render() {
    // Czyszczenie renderera przed renderowaniem nowej klatki
    SDL_RenderClear(renderer);

    // rendereowanie tła
    uint x = windowWidth / background->Width() + 1;
    uint y = windowHeight / background->Height() + 1;
    for(int j = 0; j < y; j++) {
        for(int i = 0; i < x; i++) {
            background->Render(0, 0, background->Width() * i, background->Height() * j, 0);
        }
    }

    // rendereowanie przycisków
    for(uint i = 0; i < buttons.size(); i++) {
        buttons[i]->Render();
    }

    // Rysowanie wyrenderowanej klatki
    SDL_RenderPresent(renderer);
}
