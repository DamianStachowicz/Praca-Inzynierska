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

    // Inicjalizacja stanów gry
    Texture* menuBg = new Texture(renderer, "gfx/darkPurple.png");
    menuState = MenuState(renderer, windowWidth, windowHeight, menuBg);
    Texture* defaultBtn  = new Texture(renderer, "gfx/buttonBlue.png");
    Texture* selectedBtn = new Texture(renderer, "gfx/buttonRed.png");
    std::function<void()> func = std::bind(&Engine::StartGame, this);
    Button* btn = new Button(renderer, defaultBtn, selectedBtn, font, "Graj", {0x0, 0x0, 0x0, 0xFF},
                             windowWidth / 2 - 111, 20, func);
    btn->Switch();
    menuState.AddButton(btn);
    func = std::bind(&Engine::LevelPick, this);
    btn = new Button(renderer, defaultBtn, selectedBtn, TTF_OpenFont( "ttf/kenvector_future.ttf", 20 ), "Wybierz poziom", {0x0, 0x0, 0x0, 0xFF},
                     windowWidth / 2 - 111, 80, func);
    menuState.AddButton(btn);
    func = std::bind(&Engine::EndProgram, this);
    btn = new Button(renderer, defaultBtn, selectedBtn, font, "Wyjdz", {0x0, 0x0, 0x0, 0xFF},
                     windowWidth / 2 - 111, 140, func);
    menuState.AddButton(btn);

    inGameState = InGameState(renderer, windowWidth, windowHeight);
    func = std::bind(&Engine::EndGame, this);
    inGameState.Init(player, &score, func);

    lvlPickState = LvlPickState(renderer, windowWidth, windowHeight, menuBg);
    func = std::bind(&Engine::LoadLevel, this);
    std::function<void()> func2 = std::bind(&Engine::Return2Menu, this);
    lvlPickState.Init(func, func2);

    currentState = &menuState;

    return true;
}

/** @brief Metoda uruchamiająca grę
 */
void Engine::Run() {
    SDL_Event event;
    quit = false;

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
                currentState->HandleEvent(&event);
            }
        }

        currentState->Loop();
        currentState->Render();
    }
}

bool Engine::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = xmlDoc->NewElement("score");
    element->SetText(score);
    root->InsertEndChild(element);
    element = xmlDoc->NewElement("entities");
    tinyxml2::XMLElement* subelement;
    std::string entityType;
    for(uint i = 0; i < Entity::entities.size(); i++) {
        switch(Entity::entities[i]->type) {
            case ENTITY_TYPE_ASTEROID: entityType = "Asteroid"; break;
            case ENTITY_TYPE_PARTICLE: entityType = "Particle"; break;
            case ENTITY_TYPE_PLANET: entityType = "Planet"; break;
            case ENTITY_TYPE_ROCKET: entityType = "Rocket"; break;
            case ENTITY_TYPE_SELLING_POINT: entityType = "SellingPoint"; break;
        case ENTITY_TYPE_SPACESHIP: entityType = "SpaceShip"; break;
            default: entityType = "Default";
        }
        subelement = xmlDoc->NewElement(entityType.c_str());
        if( !Entity::entities[i]->Serialize(xmlDoc, subelement) ) {
            return false;
        }
        element->InsertEndChild(subelement);
    }
    root->InsertEndChild(element);
    element = xmlDoc->NewElement("level");
    if( !Entity::level.Serialize(xmlDoc, element) ) {
        return false;
    }
    root->InsertEndChild(element);
    element = xmlDoc->NewElement("timer");
    if( !Entity::timer.Serialize(xmlDoc, element) ) {
        return false;
    }
    root->InsertEndChild(element);
    return true;
}

bool Engine::Deserialize(tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("score");
    if( element == NULL ) {
        return false;
    }
    element->QueryUnsignedText(&score);
    element = root->FirstChildElement("level");
    if( !Entity::level.Deserialize(element) ) {
        return false;
    }
    element = root->FirstChildElement("timer");
    if( !Entity::timer.Deserialize(element) ) {
        return false;
    }
    element = root->FirstChildElement("entities");
    tinyxml2::XMLElement* listElement = element->FirstChildElement();
    Uint32 tmp;
    Entity* tmpEntity;
    while(listElement != NULL) {
        if( std::string(listElement->Name()) == "Asteroid" ) {
            listElement->FirstChildElement("size")->QueryUnsignedText(&tmp);
            Asteroid* asteroid;
            switch(tmp) {
                case ASTEROID_SIZE_BIG: {
                    asteroid = new AsteroidBig();
                    asteroid->Deserialize(listElement, renderer);
                    Entity::entities.push_back(asteroid);
                    break;
                }
                case ASTEROID_SIZE_MIDDLE: {
                    asteroid = new AsteroidMiddle();
                    asteroid->Deserialize(listElement, renderer);
                    Entity::entities.push_back(asteroid);
                    break;
                }
                case ASTEROID_SIZE_SMALL:  {
                    asteroid = new AsteroidSmall();
                    asteroid->Deserialize(listElement, renderer);
                    Entity::entities.push_back(asteroid);
                    break;
                }
                default:  {
                    asteroid = new Asteroid();
                    asteroid->Deserialize(listElement, renderer);
                    Entity::entities.push_back(asteroid);
                    break;
                }
            }
        } else if( std::string(listElement->Name()) == "SpaceShip" ) {
            SpaceShip* spaceShip = new SpaceShip();
            spaceShip->Deserialize(listElement, renderer);
            Entity::entities.push_back(spaceShip);
            player = spaceShip;
        } else if( std::string(listElement->Name()) == "Particle" ) {
            Particle* particle = new Particle();
            particle->Deserialize(listElement, renderer);
            Entity::entities.push_back(particle);
        } else if( std::string(listElement->Name()) == "Planet" ) {
            Planet* planet = new Planet();
            planet->Deserialize(listElement, renderer);
            Entity::entities.push_back(planet);
        } else if( std::string(listElement->Name()) == "Rocket" ) {
            Rocket* rocket = new Rocket();
            rocket->Deserialize(listElement, renderer);
            Entity::entities.push_back(rocket);
        } else if( std::string(listElement->Name()) == "SellingPoint" ) {
            SellingPoint* sellingPoint = new SellingPoint(&score);
            sellingPoint->Deserialize(listElement, renderer);
            Entity::entities.push_back(sellingPoint);
        } else {
            tmpEntity = new Entity();
            tmpEntity->Deserialize(listElement, renderer);
            Entity::entities.push_back(tmpEntity);
        }
        listElement = listElement->NextSiblingElement();
    }

    return true;
}

void Engine::StartGame() {
    Entity::level.Reset();
    currentState = &inGameState;
}

void Engine::EndGame() {
    currentState = &menuState;
}

void Engine::LoadLevel() {
    tinyxml2::XMLDocument xmlDoc;
    std::string filename = lvlPickState.Filename();
    if(filename == "zablokowany") {
        return;
    }
    xmlDoc.LoadFile(filename.c_str());
    tinyxml2::XMLNode* root = xmlDoc.FirstChild();
    Deserialize(root);
    currentState = &inGameState;
}

void Engine::LevelPick() {
    currentState = &lvlPickState;
}

void Engine::EndProgram() {
    quit = true;
}

void Engine::Return2Menu() {
    currentState = &menuState;
}
