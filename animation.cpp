#include "animation.h"

Animation::Animation(Uint8 numberOfFrames, Uint8 framesPerSecond, Uint32 width, Uint32 height)
{
    this->currentFrame = 0;
    this->numberOfFrames = numberOfFrames;
    timeBetweenFrames = 1000 / framesPerSecond;
    this->width = width;
    this->height = height;
    lastFrameTime = SDL_GetTicks();
}

Animation::Animation() {
    Animation(1, 12, 0, 0);
}

Uint8 Animation::CurrentFrame() {
    if(SDL_GetTicks() - lastFrameTime >= timeBetweenFrames) {
        currentFrame = (currentFrame + 1) % numberOfFrames;
        lastFrameTime = SDL_GetTicks();
    }
    return currentFrame;
}

Uint8 Animation::Width() {
    return width;
}

Uint8 Animation::Height() {
    return height;
}

bool Animation::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji obiektu. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Animation><currentFrame>" << (Uint32)currentFrame;
    file << "</currentFrame><numberOfFrames>" << (Uint32)numberOfFrames;
    file << "</numberOfFrames><timeBetweenFrames>" << timeBetweenFrames;
    file << "</timeBetweenFrames><lastFrameTime>" << lastFrameTime;
    file << "</lastFrameTime><width>" << width;
    file << "</width><height>" << height;
    file << "</height></Animation>";
    return true;
}

bool Animation::Serialize(tinyxml2::XMLDocument *xmlDoc, tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = xmlDoc->NewElement("currentFrame");
    element->SetText(currentFrame);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("numberOfFrames");
    element->SetText(numberOfFrames);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("timeBetweenFrames");
    element->SetText(timeBetweenFrames);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("lastFrameTime");
    element->SetText(lastFrameTime);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("width");
    element->SetText(width);
    root->InsertEndChild(element);

    element = xmlDoc->NewElement("height");
    element->SetText(height);
    root->InsertEndChild(element);
}

bool Animation::Deserialize(tinyxml2::XMLNode *root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("currentFrame");
    Uint32 tmp;
    element->QueryUnsignedText(&tmp);
    currentFrame = tmp;

    element = root->FirstChildElement("numberOfFrames");
    element->QueryUnsignedText(&tmp);
    numberOfFrames = tmp;

    element = root->FirstChildElement("timeBetweenFrames");
    element->QueryUnsignedText(&timeBetweenFrames);

    element = root->FirstChildElement("lastFrameTime");
    element->QueryUnsignedText(&lastFrameTime);

    element = root->FirstChildElement("width");
    element->QueryUnsignedText(&width);

    element = root->FirstChildElement("height");
    element->QueryUnsignedText(&height);
}

bool Animation::Deserialize(std::ifstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji obiektu. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }

    currentFrame = (Uint8)std::stoi(XMLhelper::GetValue(file, "<currentFrame>"));
    numberOfFrames = (Uint8)std::stoi(XMLhelper::GetValue(file, "<numberOfFrames>"));
    timeBetweenFrames = (Uint32)std::stoul(XMLhelper::GetValue(file, "<timeBetweenFrames>"));
    lastFrameTime = (Uint32)std::stoul(XMLhelper::GetValue(file, "<lastFrameTime>"));
    width = (Uint32)std::stoul(XMLhelper::GetValue(file, "<width>"));
    height = (Uint32)std::stoul(XMLhelper::GetValue(file, "<height>"));
    return true;
}
