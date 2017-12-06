#include "level.h"

Level::Level()
{
    this->r = 1500;
    this->timeLimit = 60000;
    this->startTime = SDL_GetTicks();
}

Level::Level(double r) {
    this->r = r;
    this->timeLimit = 60000;
    this->startTime = SDL_GetTicks();
}

Level::Level(double r, Uint32 timeLimit) {
    this->r = r;
    this->timeLimit = timeLimit;
    this->startTime = SDL_GetTicks();
}

Uint32 Level::TimeLeft() {
    if((SDL_GetTicks() - startTime) > timeLimit) {
        return 0;
    }
    return timeLimit - (SDL_GetTicks() - startTime);
}

std::string Level::TimeLeftString() {
    std::ostringstream os;
    os << TimeLeft() / 1000;
    return os.str();
}

bool Level::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = xmlDoc->NewElement("startTime");
    element->SetText(startTime);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("timeLimit");
    element->SetText(timeLimit);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("r");
    element->SetText(r);
    root->InsertEndChild(element);

    return true;
}

bool Level::Deserialize(tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("startTime");
    element->QueryUnsignedText(&startTime);
    element = root->FirstChildElement("timeLimit");
    element->QueryUnsignedText(&timeLimit);
    element = root->FirstChildElement("r");
    element->QueryDoubleText(&r);
    return true;
}

void Level::Reset() {
    this->startTime = SDL_GetTicks();
}
