#include "timer.h"

Timer::Timer()
{
    currentTime = SDL_GetTicks();
    // domyślna liczba klatek na sekundę oraz krok czasowy
    FPS = 30;
    dt = 10;
    //stepsPerFrame = FPS / dt;
    stepsPerFrame = 3;
    lastFrameTime = SDL_GetTicks();
}

Timer::Timer(Uint8 FPS, Uint8 dt) {
    currentTime = SDL_GetTicks();
    stepsPerFrame = 1000 / dt / FPS;
    if(stepsPerFrame < 1) {
        FPS = 30;
        dt = 10;
    }
    this->FPS = FPS;
    this->dt = dt;
    stepsPerFrame = 1000 / dt / FPS;
}

Uint8 Timer::GetFPS() {
    return FPS;
}

Uint8 Timer::GetDt() {
    return dt;
}

Uint16 Timer::GetStepsPerFrame() {
    return stepsPerFrame;
}

void Timer::NextStep() {
    while(SDL_GetTicks() - currentTime < dt) {}
    currentTime = SDL_GetTicks();
}

void Timer::UpdateLastFrameTime() {
    lastFrameTime = SDL_GetTicks();
}

bool Timer::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji stopera. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Timer><FPS>" << (Uint16)FPS;
    file << "</FPS><dt>" << (Uint16)dt;
    file << "</dt><currentTime>" << currentTime;
    file << "</currentTime><stepsPerFrame>" << stepsPerFrame;
    file << "</stepsPerFrame></Timer>";
    return true;
}

bool Timer::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = xmlDoc->NewElement("FPS");
    element->SetText(FPS);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("dt");
    element->SetText(dt);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("currentTime");
    element->SetText(currentTime);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("stepsPerFrame");
    element->SetText(stepsPerFrame);
    root->InsertEndChild(element);

    return true;
}

bool Timer::Deserialize(tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("FPS");
    Uint32 tmp;
    element->QueryUnsignedText(&tmp);
    FPS = tmp;
    element = root->FirstChildElement("dt");
    element->QueryUnsignedText(&tmp);
    dt = tmp;
    element = root->FirstChildElement("currentTime");
    element->QueryUnsignedText(&currentTime);
    element = root->FirstChildElement("stepsPerFrame");
    element->QueryUnsignedText(&tmp);
    stepsPerFrame = tmp;
    return true;
}

bool Timer::Deserialize(std::ifstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji stopera. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }
    XMLhelper::SkipTag(file, "<Timer>");
    FPS = (Uint8)stoi(XMLhelper::GetValue(file, "<FPS>"));
    dt = (Uint8)stoi(XMLhelper::GetValue(file, "<dt>"));
    currentTime = stoi(XMLhelper::GetValue(file, "<currentTime>"));
    stepsPerFrame = stoi(XMLhelper::GetValue(file, "<stepsPerFrame>"));
    return true;
}
