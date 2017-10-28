#include "entity.h"
#include "planet.h"
#include "asteroid.h"
#include "sellingpoint.h"

Timer Entity::timer = Timer(60, 1);
std::vector<Entity*> Entity::entities = std::vector<Entity*>();
Level Entity::level = Level(800);
// stała grawitacyjna
double G = 0.000000000066742;

Entity::Entity()
{
    location = vector2d();
    rotation = 0;
    r = 0;
    velocity = vector2d();
    acceleration = vector2d();
    force = vector2d();
    mass = 0;
    texture = NULL;
    colliding = std::vector<Entity*>();
    type = ENTITY_TYPE_UNDEFINED;
    deathAnimationStarted = 0;
    alive = true;
    animation = Animation();
    maxSpeed = 2000;
}

Entity::~Entity() {

}

bool Entity::CheckCollision(Entity* ent) {
    // wektor odległości między środkami encji
    vector2d v = (this->location + this->CollisionCenter()) - (ent->location + ent->CollisionCenter());
    // suma promieni
    double radia = this->r + ent->r;
    /* kolizja nastąpiła, jeśli odległość między
     * środkami jest mniejsza niż suma promieni
     */
    return (v.Length() < radia);
}

void Entity::Load(SDL_Renderer *renderer, std::__cxx11::string texturePath, Sint16 frameHeight, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass) {
    texture = new Texture(renderer, texturePath);
    if(texture->Width() > frameHeight) {
        r = texture->Width() / 2;
    } else {
        r = frameHeight / 2;
    }
    animation = Animation(numberOfFrames, framesPerSecond, texture->Width(), frameHeight);
    location = initLocation;
    this->mass = mass;
    this->collisionCenter = vector2d(0, 0);
}

void Entity::Load(SDL_Renderer *renderer, std::__cxx11::string texturePath, Sint16 frameHeight, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass, vector2d collisionCenter, double r) {
    texture = new Texture(renderer, texturePath);
    animation = Animation(numberOfFrames, framesPerSecond, texture->Width(), frameHeight);
    location = initLocation;
    this->mass = mass;
    this->collisionCenter = collisionCenter;
    this->r = r;
}

void Entity::SetInitialVelocity(vector2d v) {
    velocity = v;
}

void Entity::Render(double xOffset, double yOffset) {
    double halfWidth = texture->Width() / 2;
    // Położenie i rozmiar muszą zostać dostosowane do położenia kamery (trójwymiarowego)
    double camX = Camera::camera.location.x;
    double camY = Camera::camera.location.y;
    double zoom = Camera::camera.zoom;
    double offsetX = ( (1 - zoom) * Camera::camera.windowWidth ) / 2;
    double offsetY = ( (1 - zoom) * Camera::camera.windowHeight ) / 2;
    texture->Render(0, animation.CurrentFrame() * animation.Height(),     // srcX, srcY
                    animation.Width(), animation.Height(),                // srcWidth, srcHeight
                    (location.x - halfWidth + camX + xOffset) * zoom + offsetX,           // destX
                    (location.y - halfWidth + camY + yOffset) * zoom + offsetY,           // destY
                    animation.Width() * zoom, animation.Height() * zoom,  // destWidth, destHeight
                    rotation, SDL_FLIP_NONE);
}

void Entity::Render() {
    Render(0, 0);
}

vector2d Entity::AttractTo(Entity* ent) {
    vector2d dist = ent->location - this->location;
    if(dist.Length() != 0) {
        return dist.Direction() * ( G * this->mass * ent->mass / (dist.Length() * dist.Length()) );
    } else {
        return vector2d();
    }
}

void Entity::UpdateAcceleration() {
    acceleration = force.Direction() * (force.Length() / mass);
}

void Entity::Move() {
    Uint8 dt = timer.GetDt();
    acceleration = force / mass;
    velocity = velocity + acceleration * dt;
    // ograniczenie prędkości
    if(velocity.Length() > maxSpeed) {
        double ratio = velocity.Length() / maxSpeed;
        velocity = velocity / ratio;
    }
    vector2d shift = velocity * dt;
    location = location + shift;
    if( location.x > level.r || location.x < -level.r  ) {
        location.x = -location.x;
    }
    if( location.y > level.r || location.y < -level.r ) {
        location.y = -location.y;
    }
}

// wykrywanie kolizji
void Entity::CheckCollisions() {
    // sprawdzanie wszystkich encji
    for(uint i = 0; i < entities.size(); i++) {
        // pominięcie samej siebie
        if(entities[i] != this && CheckCollision(entities[i])) {
            // jeżeli wykryto kolizję to encja jest wrzucana do kolejki do obsłużenia zderzenia
            colliding.push_back(entities[i]);
        }
    }
}

void Entity::UpdateForce() {
    vector2d force = vector2d(0, 0);
    for(uint i = 0; i < entities.size(); i++) {
        if(entities[i] != this) {
            force = force + AttractTo(entities[i]);
        }
    }
    this->force = force;
}

void Entity::Collide() {
}

bool Entity::Alive() {
    return alive;
}

bool Entity::DeathStarted() {
    return !(deathAnimationStarted == 0);
}

void Entity::Loop() {
    UpdateForce();
    Move();
}

vector2d Entity::Momentum() {
    return velocity * mass;
}

void Entity::Bounce(Entity *ent) {
    //velocity = ( velocity*(mass - ent->mass) + ent->velocity*2*ent->mass ) / (mass + ent->mass);
    double massFactor = 2 * ent->mass / (mass + ent->mass);
    vector2d loc = (location - ent->location);
    double a = (velocity - ent->velocity) * loc;
    double b = loc.Length() * loc.Length();
    velocity = velocity - loc * (massFactor * a / b);
}

vector2d Entity::GetMinimapLocation(double ratio) {
    return location * ratio;
}

double Entity::GetMinimapR(double ratio) {
    return r * ratio;
}

void Entity::RenderCopy(int position) {
    switch(position) {
        case LEVEL_COPY_MIDDLE_BOTTOM: Render(0, 2 * level.r); break;
        case LEVEL_COPY_LEFT_BOTTOM: Render(-2 * level.r, 2 * level.r); break;
        case LEVEL_COPY_LEFT_MIDDLE: Render(-2 * level.r, 0); break;
        case LEVEL_COPY_LEFT_TOP: Render(-2 * level.r, -2 * level.r); break;
        case LEVEL_COPY_MIDDLE_TOP: Render(0, -2 * level.r);  break;
        case LEVEL_COPY_RIGHT_TOP: Render(2 * level.r, -2 * level.r);  break;
        case LEVEL_COPY_RIGHT_MIDDLE: Render(2 * level.r, 0);  break;
        case LEVEL_COPY_RIGHT_BOTTOM: Render(2 * level.r, 2 * level.r);  break;
    }
}

GfxData::GfxData(){

}

std::vector<GfxData> Entity::GetGfxData(std::__cxx11::string cfgFile) {
    std::vector<GfxData> GfxDataList;
    std::ifstream list(cfgFile);
    if(list.is_open()) {
        std::string line;
        while(getline(list, line)) {
            GfxDataList.push_back(GfxData(line));
        }
    } else {
        std::cerr << "Nie udało się pobrać listy grafik z pliku " << cfgFile << std::endl;
    }
    return GfxDataList;
}

void Entity::GenerateLevel(SDL_Renderer* renderer, Uint32 *score, double size, Uint32 timeLimit, Uint8 numberOfPlanets,
                           Uint8 numberOfAsteroids, Uint8 numberOfSellingPoints, Uint8 seed) {
    // czyszczenie wektora encji
    entities.clear();
    level = Level(size, timeLimit);

    // pobieranie listy grafik
    std::vector<GfxData> gfxData = Entity::GetGfxData("gfx.txt");
    std::vector<GfxData> planetGfx = Entity::FilterGfxData(gfxData, ENTITY_TYPE_PLANET);
    std::vector<GfxData> asteroidGfx = Entity::FilterGfxData(gfxData, ENTITY_TYPE_ASTEROID);
    std::vector<GfxData> sellingPointGfx = Entity::FilterGfxData(gfxData, ENTITY_TYPE_SELLING_POINT);

    // inicjowanie generatora liczb losowych
    srand(seed);

    GfxData entityGfxData;
    int index;
    double maxVelocity = 2; // wzdłuż osi
    double planetDensity = 100; // kg/px^3
    double asteroidDensity = 200; // kg/px^3
    double sellingPointDensity = 10; // kg/px^3
    double mass;
    vector2d location;
    Entity* tmpEntity;

    // dodawanie planet
    for(int i = 0; i < numberOfPlanets; i++) {
        // losowanie grafiki planety
        if(!planetGfx.size()) {
            std::cerr << "Brak informacji o grafikach planet w pliku konfiguracyjnym" << std::endl;
            return;
        }
        index = rand() % planetGfx.size();
        entityGfxData = planetGfx[index];
        // obliczanie masy
        mass = 0.75 * M_PI * pow((double)entityGfxData.frameHeight / 2, 3) * planetDensity; // planeta jest kulą o zadanej gęstości
        // losowanie położenia
        location = vector2d((double)rand() / RAND_MAX * size, (double)rand() / RAND_MAX * size);

        // inicjowanie nowej encji
        tmpEntity = new Planet();
        tmpEntity->Load(renderer, entityGfxData.path, entityGfxData.frameHeight, entityGfxData.numberOfFrames,
                        entityGfxData.numberOfFrames, location, mass);
        // upewnienie się, że encja z niczym nie koliduje
        int safety = 0; // dodatkowe zabezpieczenie
        tmpEntity->CheckCollisions();
        while(tmpEntity->colliding.size() > 0) {
            location = vector2d((double)rand() / RAND_MAX * size, (double)rand() / RAND_MAX * size);
            tmpEntity->Load(renderer, entityGfxData.path, entityGfxData.frameHeight, entityGfxData.numberOfFrames,
                            entityGfxData.numberOfFrames, location, mass);
            if(safety > 1000) {
                std::cerr << "Poziom zbyt mały lub za dużo obiektów." << std::endl;
                return;
            }
            ++safety;
        }
        // losowanie początkowej prędkości
        tmpEntity->SetInitialVelocity(vector2d((double)rand() / RAND_MAX * maxVelocity,
                                               (double)rand() / RAND_MAX * maxVelocity));
        // dodawanie encji
        entities.push_back(tmpEntity);
    }

    // dodawanie asteroid
    for(int i = 0; i < numberOfAsteroids; i++) {
        // losowanie grafiki asteroidy
        if(!asteroidGfx.size()) {
            std::cerr << "Brak informacji o grafikach asteroid w pliku konfiguracyjnym" << std::endl;
            return;
        }
        index = rand() % asteroidGfx.size();
        entityGfxData = asteroidGfx[index];
        // obliczanie masy
        mass = 0.75 * M_PI * pow((double)entityGfxData.frameHeight / 2, 3) * asteroidDensity; // planeta jest kulą o zadanej gęstości
        // losowanie położenia
        location = vector2d((double)rand() / RAND_MAX * size, (double)rand() / RAND_MAX * size);

        // inicjowanie nowej encji
        tmpEntity = new Asteroid();
        tmpEntity->Load(renderer, entityGfxData.path, entityGfxData.frameHeight, entityGfxData.numberOfFrames,
                        entityGfxData.numberOfFrames, location, mass);
        // upewnienie się, że encja z niczym nie koliduje
        int safety = 0; // dodatkowe zabezpieczenie
        tmpEntity->CheckCollisions();
        while(tmpEntity->colliding.size() > 0) {
            location = vector2d((double)rand() / RAND_MAX * size, (double)rand() / RAND_MAX * size);
            tmpEntity->Load(renderer, entityGfxData.path, entityGfxData.frameHeight, entityGfxData.numberOfFrames,
                            entityGfxData.numberOfFrames, location, mass);
            if(safety > 1000) {
                std::cerr << "Poziom zbyt mały lub za dużo obiektów." << std::endl;
                return;
            }
            ++safety;
        }
        // losowanie początkowej prędkości
        tmpEntity->SetInitialVelocity(vector2d((double)rand() / RAND_MAX * maxVelocity,
                                               (double)rand() / RAND_MAX * maxVelocity));
        // dodawanie encji
        entities.push_back(tmpEntity);
    }

    // dodawanie punktów sprzedarzy
    for(int i = 0; i < numberOfAsteroids; i++) {
        // losowanie grafiki asteroidy
        if(!sellingPointGfx.size()) {
            std::cerr << "Brak informacji o grafikach punktów sprzedaży w pliku konfiguracyjnym" << std::endl;
            return;
        }
        index = rand() % sellingPointGfx.size();
        entityGfxData = sellingPointGfx[index];
        // obliczanie masy
        mass = 0.75 * M_PI * pow((double)entityGfxData.frameHeight / 2, 3) * sellingPointDensity; // planeta jest kulą o zadanej gęstości
        // losowanie położenia
        location = vector2d((double)rand() / RAND_MAX * size, (double)rand() / RAND_MAX * size);

        // inicjowanie nowej encji
        tmpEntity = new SellingPoint(score);
        tmpEntity->Load(renderer, entityGfxData.path, entityGfxData.frameHeight, entityGfxData.numberOfFrames,
                        entityGfxData.numberOfFrames, location, mass);
        // upewnienie się, że encja z niczym nie koliduje
        int safety = 0; // dodatkowe zabezpieczenie
        tmpEntity->CheckCollisions();
        while(tmpEntity->colliding.size() > 0) {
            location = vector2d((double)rand() / RAND_MAX * size, (double)rand() / RAND_MAX * size);
            tmpEntity->Load(renderer, entityGfxData.path, entityGfxData.frameHeight, entityGfxData.numberOfFrames,
                            entityGfxData.numberOfFrames, location, mass);
            if(safety > 1000) {
                std::cerr << "Poziom zbyt mały lub za dużo obiektów." << std::endl;
                return;
            }
            ++safety;
        }
        // dodawanie encji
        entities.push_back(tmpEntity);
    }
}

GfxData::GfxData(std::__cxx11::string data) {
    std::string tmpString = "";
    const char* dataChars = data.c_str();

    // ścieżka do obrazka
    Uint16 i = 0;
    while(dataChars[i] != ',') {
        tmpString += dataChars[i];
        ++i;
    }
    path = tmpString;

    // klasa obiektu
    tmpString = "";
    ++i;
    while(dataChars[i] != ',') {
        tmpString += dataChars[i];
        ++i;
    }
    entityType = Entity::EntityTypeCode(tmpString);

    // wysokość klatki
    tmpString = "";
    ++i;
    while(dataChars[i] != ',') {
        tmpString += dataChars[i];
        ++i;
    }
    frameHeight = (Uint16)std::stoi(tmpString);

    // liczba klatek
    tmpString = "";
    ++i;
    while(dataChars[i] != ',') {
        tmpString += dataChars[i];
        ++i;
    }
    numberOfFrames = (Uint8)std::stoi(tmpString);

    // liczba klatek na sekundę
    tmpString = "";
    ++i;
    while(dataChars[i] != '\0') {
        tmpString += dataChars[i];
        ++i;
    }
    framesPerSecond = (Uint8)std::stoi(tmpString);
}

Uint8 Entity::EntityTypeCode(std::__cxx11::string name) {
    if(name == "SPACESHIP") {
        return ENTITY_TYPE_SPACESHIP;
    }
    if(name == "PLANET") {
        return ENTITY_TYPE_PLANET;
    }
    if(name == "ASTEROID") {
        return ENTITY_TYPE_ASTEROID;
    }
    if(name == "ROCKET") {
        return ENTITY_TYPE_ROCKET;
    }
    if(name == "PARTICLE") {
        return ENTITY_TYPE_PARTICLE;
    }
    if(name == "SELLINGPOINT") {
        return ENTITY_TYPE_SELLING_POINT;
    }
    return ENTITY_TYPE_UNDEFINED;
}

std::vector<GfxData> Entity::FilterGfxData(std::vector<GfxData> gfxData, Uint8 type) {
    std::vector<GfxData> filteredData;
    for(uint i = 0; i < gfxData.size(); ++i) {
        if(gfxData[i].entityType == type) {
            filteredData.push_back(gfxData[i]);
        }
    }
    return filteredData;
}

bool Entity::Serialize(std::ofstream& file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji obiektu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Entity><r>" << r;
    file << "</r><velocity>" << velocity.x << ":" << velocity.y;
    file << "</velocity><acceleration>" << acceleration.x << ":" << acceleration.y;
    file << "</acceleration><force>" << force.x << ":" << force.y;
    file << "</force><texture>" << texture->path;
    file << "</texture><deathAnimationStarted>" << deathAnimationStarted;
    file << "</deathAnimationStarted><alive>" << alive;
    file << "</alive><location>" << location.x << ":" << location.y;
    file << "</location><rotation>" << rotation;
    file << "</rotation><type>" << (Uint32)type;
    file << "</type><mass>" << mass;
    file << "</mass><collisionCenter>" << collisionCenter.x << ":" << collisionCenter.y;
    file << "</collisionCenter>";
    animation.Serialize(file);
    file << "</Entity>";
    return true;
}

bool Entity::Deserialize(std::ifstream& file, SDL_Renderer* renderer) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji obiektu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }

    std::string tmpStr;
    r = std::stod(XMLhelper::GetValue(file, "<r>"));
    tmpStr = XMLhelper::GetValue(file, "<velocity>");
    velocity = vector2d(tmpStr);
    tmpStr = XMLhelper::GetValue(file, "<acceleration>");
    acceleration = vector2d(tmpStr);
    tmpStr = XMLhelper::GetValue(file, "<force>");
    force = vector2d(tmpStr);
    texture = new Texture(renderer, XMLhelper::GetValue(file, "<texture>"));
    deathAnimationStarted = stoi(XMLhelper::GetValue(file, "<deathAnimationStarted>"));
    if(XMLhelper::GetValue(file, "<alive>") == "1") {
        alive = true;
    } else {
        alive = false;
    }
    tmpStr = XMLhelper::GetValue(file, "<location>");
    location = vector2d(tmpStr);
    rotation = std::stod(XMLhelper::GetValue(file, "<rotation>"));
    type = (Uint8)std::stoi(XMLhelper::GetValue(file, "<type>"));
    mass = std::stod(XMLhelper::GetValue(file, "<mass>"));
    collisionCenter = XMLhelper::GetValue(file, "<collisionCenter>");
    XMLhelper::SkipTag(file, "<Animation>");
    animation.Deserialize(file);
    XMLhelper::SkipTag(file, "</Animation>");
    XMLhelper::SkipTag(file, "</Entity>");
    return true;
}

vector2d Entity::CollisionCenter() {
    double angle = rotation * M_PI / 180;
    return vector2d(collisionCenter.x * cos(angle) - collisionCenter.y * sin(angle),
                    collisionCenter.x * sin(angle) + collisionCenter.y * cos(angle));
}
