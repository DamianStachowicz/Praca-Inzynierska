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
    scoreColor = {0x0, 0x0, 0x0, 0xFF};
    scoreBg = NULL;
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
    font = TTF_OpenFont( "ttf/kenvector_future.ttf", 30 );
    if( font == NULL ) {
        std::cerr << "Engine::Engine() Błąd, nie udało się załadować czcionki. Błąd SDL_ttf: " << TTF_GetError() << std::endl;
    }

    // Tworzenie okna
    window = SDL_CreateWindow( "Stachowicz Damian Praca Dyplomowa 0.3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    // | SDL_WINDOW_FULLSCREEN_DESKTOP
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

    std::ifstream ifile;
    //ifile.open("Test-2-asteroids.xml");
    ifile.open("test1.xml");
    //ifile.open("planet.xml");
    Deserialize(ifile);
    ifile.close();

    // Inicjalizacja stanów gry
    inGameState = InGameState(renderer, windowWidth, windowHeight);
    inGameState.Init(player);
    currentState = &inGameState;

    return true;
}

/** @brief Metoda uruchamiająca grę
 */
void Engine::Run() {
    currentState->Run();
}

/** @brief Główna pętla, logika gry
 */
void Engine::Loop() {
    currentState->Loop();
}

/** @brief Metoda renderująca obiekty
 */
void Engine::Render() {
    currentState->Render();
}

void Engine::HandleEvent(SDL_Event* event)
{

}

void Engine::HandleKeyDown()
{

}

void Engine::HandleKeyUp()
{

}

void Engine::HandleMouseMotion(SDL_MouseMotionEvent* event) {

}

void Engine::HandleMouseButtonDown(SDL_MouseButtonEvent *event) {

}

void Engine::HandleMouseWheelScroll(SDL_MouseWheelEvent *event) {

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
