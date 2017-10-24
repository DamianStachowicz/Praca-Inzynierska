#include "level.h"

Level::Level()
{
    this->r = 1500;
    this->timeLimit = 60000;
    startTime = SDL_GetTicks();
}

Level::Level(double r) {
    this->r = r;
    this->timeLimit = 60000;
    startTime = SDL_GetTicks();
}

Level::Level(double r, Uint32 timeLimit) {
    this->r = r;
    this->timeLimit = timeLimit;
    startTime = SDL_GetTicks();
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

bool Level::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji poziomu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Level><startTime>" << startTime;
    file << "</startTime><timeLimit>" << timeLimit;
    file << "</timeLimit><r>" << r;
    file << "</r></Level>";
    return true;
}

bool Level::Deserialize(std::ifstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji poziomu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }
    XMLhelper::SkipTag(file, "<Level>");
    startTime = std::stoul(XMLhelper::GetValue(file, "<startTime>"));
    timeLimit = std::stoul(XMLhelper::GetValue(file, "<timeLimit>"));
    r = std::stod(XMLhelper::GetValue(file, "<r>"));
    XMLhelper::SkipTag(file, "</Level>");
    return true;
}
