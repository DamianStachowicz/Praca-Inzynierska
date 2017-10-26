/** @file engine.cpp
 *  @author Damian Stachowicz
 *  @brief Implementacja silnika gry
 */
#include "engine.h"

/** @brief Konstruktor domyślny, inicjalizacja zmiennych
 */
Engine::Engine()
{
    // Inicjalizacja zmiennych
    window   = NULL;
    renderer = NULL;
    windowWidth  = 1200;
    windowHeight = 720;
    keyStates = SDL_GetKeyboardState( NULL );
    Camera::camera.SetSize(windowWidth, windowHeight);
    player = new SpaceShip();
    score = 0;
    // ładowanie fontów
    font = NULL;
    scoreTexture = NULL;
    scoreColor = {0xFF, 0xFF, 0x00, 0xFF};
    timerTexture = NULL;
    timerColor = {0xFF, 0xFF, 0x00, 0xFF};
}

/** @brief Destruktor
 */
Engine::~Engine()
{
    // zwolnienie fonta
    TTF_CloseFont(font);
    font = NULL;

    // niszczenie okna
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    // zamknięcie SDL, zwolnienie pamięci
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/** @brief Inicjalizacja silnika
 *
 *  Metoda inicjalizująca silinik, inicjalizuje SDL oraz odpowiednie dodatki,
 *  wczytuje mapę oraz obiekty
 */
bool Engine::Init() {
    // Inicjalizacja SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        std::cerr << "Engine::Init Błąd, nie udało się zainicjalizować SDL. Błąd SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Inicjalizacja SDL_image
    if( IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG ) {
        std::cerr << "Engine::Init Błąd, nie udało się zainicjalizować SDL_image. Błąd SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    // Inicjalizacja SDL_ttf
    if( TTF_Init() == -1 ) {
        std::cerr << "Engine::Init Błąd, nie udało się zainicjalizować SDL_ttf. Błąd SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }
    // Ładowanie czcionki
    font = TTF_OpenFont( "ttf/Gputeks-Regular.ttf", 14 );
    if( font == NULL ) {
        std::cerr << "Engine::Engine() Błąd, nie udało się załadować czcionki. Błąd SDL_ttf: " << TTF_GetError() << std::endl;
    }

    // Tworzenie okna
    window = SDL_CreateWindow( "Stachowicz Damian Praca Dyplomowa 0.3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if( window == NULL )
    {
        std::cerr << "Engine::Init Błąd, nie udało się utworzyć okna. Błąd SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tworzenie renderera
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if( renderer == NULL )
    {
        std::cerr << "Engine::Init Błąd, nie udało się utworzyć renderera. Błąd SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Ustawianie koloru tła
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );

    // Ustawienie tekstu wyniku
    scoreTexture = new Texture(renderer, "Wynik: 0", font, scoreColor);
    timerTexture = new Texture(renderer, Entity::level.TimeLeftString(), font, timerColor);

    std::ifstream ifile;
    //ifile.open("Test-2-asteroids.xml");
    ifile.open("test1.xml");
    //ifile.open("planet.xml");
    Deserialize(ifile);
    ifile.close();

    return true;
}

/** @brief Metoda uruchamiająca grę
 */
void Engine::Run() {
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

/** @brief Główna pętla, logika gry
 */
void Engine::Loop() {
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
    scoreTexture = new Texture(renderer, "Wynik: " + std::to_string(score) + " Wytrzymałość statku: " + std::to_string(player->Health())
                               , font, scoreColor);
    // aktualizacja licznika czasu
    free(timerTexture);
    timerTexture = new Texture(renderer, Entity::level.TimeLeftString(), font, timerColor);
}

/** @brief Metoda renderująca obiekty
 */
void Engine::Render() {
    // Czyszczenie renderera przed renderowaniem nowej klatki
    SDL_RenderClear(renderer);

    // Renderowanie encji
    for(uint i = 0; i < Entity::entities.size(); i++) {
        Entity::entities[i]->Render();

        // Renderowanie kopii wszystkich encji, w celu zachowania iluzji cykliczności
        // należy pamiętać o tym, że strzałka y wskazuje w dół
        if(player->location.x < 0) {
            if(player->location.y > 0) {
                Entity::entities[i]->RenderCopy(LEVEL_COPY_MIDDLE_BOTTOM);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_LEFT_BOTTOM);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_LEFT_MIDDLE);
            } else {
                Entity::entities[i]->RenderCopy(LEVEL_COPY_LEFT_MIDDLE);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_LEFT_TOP);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_MIDDLE_TOP);
            }
        } else {
            if(player->location.y > 0) {
                Entity::entities[i]->RenderCopy(LEVEL_COPY_RIGHT_MIDDLE);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_RIGHT_BOTTOM);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_MIDDLE_BOTTOM);
            } else {
                Entity::entities[i]->RenderCopy(LEVEL_COPY_MIDDLE_TOP);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_RIGHT_TOP);
                Entity::entities[i]->RenderCopy(LEVEL_COPY_RIGHT_MIDDLE);
            }
        }
    }

    // Rysowanie interfejsu
    scoreTexture->Render(0, 0, 10, 10, 0);
    timerTexture->Render(0, 0, windowWidth / 2 - 10, 10, 0);

    // Rysowanie wyrenderowanej klatki
    SDL_RenderPresent(renderer);

    // Aktualizacja czasu wyświetlenia ostatniej ramki
    Entity::timer.UpdateLastFrameTime();
}

void Engine::HandleEvent(SDL_Event* event)
{
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

void Engine::HandleKeyDown()
{
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

void Engine::HandleKeyUp()
{
    if( !keyStates[ SDL_SCANCODE_S ] ) {
        player->stop = false;
    } if( !keyStates[ SDL_SCANCODE_W ] )
    {
        player->advance = false;
    }
}

void Engine::HandleMouseMotion(SDL_MouseMotionEvent* event) {
    double mouseX = event->x - (Sint32)(windowWidth  / 2);
    double mouseY = event->y - (Sint32)(windowHeight / 2);
    double angle = atan2(mouseY, mouseX);
    player->Rotate(angle);
}

void Engine::HandleMouseButtonDown(SDL_MouseButtonEvent *event) {
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

void Engine::HandleMouseWheelScroll(SDL_MouseWheelEvent *event) {
    Camera::camera.zoom += (double)event->y / 10;
}

bool Engine::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji silnika. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }

    file << "<Engine><score>" << score;
    file << "</score><nrOfEntities>" << Entity::entities.size() << "</nrOfEntities>";
    for(uint i = 0; i < Entity::entities.size(); i++) {
        file << "<entityType>" << (Uint16)Entity::entities[i]->type << "</entityType>";
        if( !Entity::entities[i]->Serialize(file) ) {
            return false;
        }
    }
    if( !Entity::level.Serialize(file) ) {
        return false;
    }
    if( !Entity::timer.Serialize(file) ) {
        return false;
    }
    file << "</Engine>";
    return true;
}

bool Engine::Deserialize(std::ifstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji silnika. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }

    XMLhelper::SkipTag(file, "<Engine>");
    score = (Uint32)std::stoul(XMLhelper::GetValue(file, "<score>"));
    int nrOfEntities = stoi(XMLhelper::GetValue(file, "<nrOfEntities>"));
    Entity::entities.clear();
    Uint8 type;
    for(int i = 0; i < nrOfEntities; i++) {
        type = (Uint8)std::stoi(XMLhelper::GetValue(file, "<entityType>"));
        switch(type) {
            case ENTITY_TYPE_ASTEROID:
            {
                Uint8 size = (Uint8)std::stoi(XMLhelper::GetValue(file, "<size>"));
                Asteroid *tmp;
                switch(size) {
                    case ASTEROID_SIZE_BIG: tmp = new AsteroidBig(); break;
                    case ASTEROID_SIZE_MIDDLE: tmp = new AsteroidMiddle(); break;
                    case ASTEROID_SIZE_SMALL: tmp = new AsteroidSmall(); break;
                }
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                break;
            }
            case ENTITY_TYPE_PARTICLE:
            {
                Particle* tmp = new Particle();
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                break;
            }
            case ENTITY_TYPE_PLANET:
            {
                Planet* tmp = new Planet();
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                break;
            }
            case ENTITY_TYPE_ROCKET:
            {
                Rocket* tmp = new Rocket();
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                break;
            }
            case ENTITY_TYPE_SELLING_POINT:
            {
                SellingPoint* tmp = new SellingPoint(&score);
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                break;
            }
            case ENTITY_TYPE_SPACESHIP:
            {
                SpaceShip* tmp = new SpaceShip();
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                player = tmp;
                break;
            }
            default:
            {
                Entity* tmp = new Entity();
                Entity::entities.push_back(tmp);
                tmp->Deserialize(file, renderer);
                break;
            }
        }
    }

    int playerIdx = std::stoi(XMLhelper::GetValue(file, "<playerIdx>"));
    player = (SpaceShip*)(Entity::entities[playerIdx]);

    Entity::level.Deserialize(file);
    Entity::timer.Deserialize(file);
    return true;
}
