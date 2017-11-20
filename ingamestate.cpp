#include "ingamestate.h"

InGameState::InGameState() {
    InGameState(NULL, 0, 0);
}

InGameState::InGameState(SDL_Renderer *renderer, Uint32 windowWidth, Uint32 windowHeight) {
    this->renderer = renderer;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->player = player;
}

void InGameState::Run() {
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

        if(player->Health() <= 0) {
            std::cout << "Statek zniszczony. Położenie: " << player->location.x << ":" << player->location.y << std::endl;
            quit = true;
        }

        Loop();
        Render();
    }
}

void InGameState::Init(SpaceShip* player) {
    keyStates = SDL_GetKeyboardState( NULL );
    score = 0;
    this->player = player;
    // ładowanie fontów
    font = NULL;
    scoreTexture = NULL;
    scoreColor = {0x0, 0x0, 0x0, 0xFF};
    scoreBg = NULL;
    timerTexture = NULL;
    timerColor = {0xFF, 0xFF, 0x00, 0xFF};

    // Inicjalizacja SDL_ttf
    if( TTF_Init() == -1 ) {
        std::cerr << "Engine::Init Błąd, nie udało się zainicjalizować SDL_ttf. Błąd SDL_ttf: " << TTF_GetError() << std::endl;
        return;
    }

    // Ładowanie czcionki
    font = TTF_OpenFont( "ttf/kenvector_future.ttf", 30 );
    if( font == NULL ) {
        std::cerr << "Engine::Engine() Błąd, nie udało się załadować czcionki. Błąd SDL_ttf: " << TTF_GetError() << std::endl;
        return;
    }

    // Ustawienie tekstu wyniku
    scoreTexture = new Texture(renderer, "0", font, scoreColor);
    scoreBg = new Texture(renderer, "gfx/buttonBlue.png");
    timerTexture = new Texture(renderer, Entity::level.TimeLeftString(), font, timerColor);
}

void InGameState::Loop() {
    while( (SDL_GetTicks() - Entity::timer.lastFrameTime) < (1000 / Entity::timer.GetFPS()) ) {
        // usuwanie martwych obiektów
        for(uint j = 0; j < Entity::entities.size(); j++) {
            if(!Entity::entities[j]->Alive()) {
                delete(Entity::entities[j]);
                Entity::entities.erase(Entity::entities.begin() + j);
            }
        }
        // Obliczenia dla pojedynczego kroku
        // Przesuwanie wszystkich encji
        for(uint j = 0; j < Entity::entities.size(); j++) {
            Entity::entities[j]->Loop();
        }
        // Dopiero po przesunięciu wszystkich encji można sprawdzić które kolidują
        for(uint j = 0; j < Entity::entities.size(); j++) {
            Entity::entities[j]->CheckCollisions();
        }
        // Dopiero po sprawdzeniu które encje kolidują można zaregować na kolizje
        for(uint j = 0; j < Entity::entities.size(); j++) {
            Entity::entities[j]->Collide();
        }

        Entity::timer.NextStep();
    }

    // aktualizacja wyniku
    free(scoreTexture);
    scoreTexture = new Texture(renderer, std::to_string(score), font, scoreColor);
    // aktualizacja licznika czasu
    free(timerTexture);
    timerTexture = new Texture(renderer, Entity::level.TimeLeftString(), font, timerColor);
}

void InGameState::Render() {
    // Czyszczenie renderera przed renderowaniem nowej klatki
    SDL_RenderClear(renderer);

    // Renderowanie encji
    for(uint i = 0; i < Entity::entities.size(); i++) {
        Entity* ent = Entity::entities[i];
        ent->Render();

        // Renderowanie kopii wszystkich encji, w celu zachowania iluzji cykliczności
        // należy pamiętać o tym, że strzałka y wskazuje w dół
        if(player->location.x < 0) {
            if(player->location.y > 0) {
                ent->RenderCopy(LEVEL_COPY_MIDDLE_BOTTOM);
                ent->RenderCopy(LEVEL_COPY_LEFT_BOTTOM);
                ent->RenderCopy(LEVEL_COPY_LEFT_MIDDLE);
            } else {
                ent->RenderCopy(LEVEL_COPY_LEFT_MIDDLE);
                ent->RenderCopy(LEVEL_COPY_LEFT_TOP);
                ent->RenderCopy(LEVEL_COPY_MIDDLE_TOP);
            }
        } else {
            if(player->location.y > 0) {
                ent->RenderCopy(LEVEL_COPY_RIGHT_MIDDLE);
                ent->RenderCopy(LEVEL_COPY_RIGHT_BOTTOM);
                ent->RenderCopy(LEVEL_COPY_MIDDLE_BOTTOM);
            } else {
                ent->RenderCopy(LEVEL_COPY_MIDDLE_TOP);
                ent->RenderCopy(LEVEL_COPY_RIGHT_TOP);
                ent->RenderCopy(LEVEL_COPY_RIGHT_MIDDLE);
            }
        }
    }

    // Rysowanie interfejsu
    scoreBg->Render(0, 0, 10, 10, 0);
    scoreTexture->Render(0, 0, 15, 11, 0);
    timerTexture->Render(0, 0, windowWidth / 2 - 10, 10, 0);

    // Rysowanie wyrenderowanej klatki
    SDL_RenderPresent(renderer);

    // Aktualizacja czasu wyświetlenia ostatniej ramki
    Entity::timer.UpdateLastFrameTime();
}

void InGameState::HandleEvent(SDL_Event *event) {
    keyStates = SDL_GetKeyboardState( NULL );
    switch( event->type )
    {
        case SDL_KEYDOWN: HandleKeyDown(); break;
        case SDL_KEYUP:   HandleKeyUp(); break;
        case SDL_MOUSEMOTION: HandleMouseMotion((SDL_MouseMotionEvent*)event); break;
        case SDL_MOUSEBUTTONDOWN: HandleMouseButtonDown((SDL_MouseButtonEvent*)event); break;
        case SDL_MOUSEWHEEL: HandleMouseWheelScroll((SDL_MouseWheelEvent*)event); break;
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

void InGameState::HandleKeyDown() {
    if( keyStates[ SDL_SCANCODE_S ] ) {
        player->stop = true;
    } else if( keyStates[ SDL_SCANCODE_W ] ) {
        player->advance = true;
    }
    if( keyStates[ SDL_SCANCODE_E ] ) {
        Camera::camera.zoom += 0.01;
    } else if ( keyStates[ SDL_SCANCODE_Q ] ){
        Camera::camera.zoom -= 0.01;
    }
}

void InGameState::HandleKeyUp() {
    if( !keyStates[ SDL_SCANCODE_S ] ) {
        player->stop = false;
    } if( !keyStates[ SDL_SCANCODE_W ] )
    {
        player->advance = false;
    }
}

void InGameState::HandleMouseButtonDown(SDL_MouseButtonEvent *event) {
    if(event->button == SDL_BUTTON_LEFT) {
        if(player->rocket == NULL || !(player->rocket->Alive())) {
            double mouseX = event->x - (Sint32)(windowWidth  / 2);
            double mouseY = event->y - (Sint32)(windowHeight / 2);
            vector2d direction = vector2d(mouseX, mouseY);
            vector2d velocity = direction / direction.Length() * 0.5;
            player->Shoot(renderer, velocity);
        } else {
            player->rocket->Explode();
        }
    }
}

void InGameState::HandleMouseMotion(SDL_MouseMotionEvent *event) {
    double mouseX = event->x - (Sint32)(windowWidth  / 2);
    double mouseY = event->y - (Sint32)(windowHeight / 2);
    double angle = atan2(mouseY, mouseX);
    player->Rotate(angle);
}

void InGameState::HandleMouseWheelScroll(SDL_MouseWheelEvent *event) {
    Camera::camera.zoom += (double)event->y / 10;
}
