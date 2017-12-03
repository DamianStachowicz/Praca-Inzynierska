#include "camera.h"

Camera Camera::camera = Camera();

Camera::Camera()
{
    location.x = 0;
    location.y = 0;
    zoom = 1;
    windowHeight = 0;
    windowWidth = 0;
}

void Camera::Move(vector2d shift) {
    location = location + shift;
}

void Camera::SetSize(Uint32 width, Uint32 height) {
    windowHeight = height;
    windowWidth  = width;
}

bool Camera::Serialize(std::ofstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby serializacji kamery. Plik nie jest otwarty do zapisu." << std::endl;
        return false;
    }
    file << "<Camera><location>" << location.x << ":" << location.y;
    file << "</location><zoom>" << zoom;
    file << "</zoom><windowWidth>" << windowWidth;
    file << "</windowWidth><windowHeight>" << windowHeight;
    file << "</windowHeight></Camera>";
    return true;
}

bool Camera::Deserialize(std::ifstream &file) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby deserializacji kamera. Plik nie jest otwarty do odczytu." << std::endl;
        return false;
    }

    std::string tmpStr;
    XMLhelper::SkipTag(file, "<Camera>");
    tmpStr = XMLhelper::GetValue(file, "<location>");
    location = vector2d(tmpStr);
    zoom = std::stod(XMLhelper::GetValue(file, "<zoom>"));
    windowWidth = (Uint32)std::stoi(XMLhelper::GetValue(file, "<windowWidth>"));
    windowHeight = (Uint32)std::stoi(XMLhelper::GetValue(file, "<windowHeight>"));
    XMLhelper::SkipTag(file, "</Camera>");
    return true;
}

double Camera::LeftEdge(double levelR) {
    double zoom = 1 / this->zoom;
    if(location.x - (windowWidth / 2) * zoom < -levelR) {
        return location.x - (windowWidth / 2) * zoom + (2 * levelR);
    } else {
        return location.x - (windowWidth / 2) * zoom;
    }
}

double Camera::RightEdge(double levelR) {
    double zoom = 1 / this->zoom;
    if(location.x + (windowWidth / 2) * zoom > levelR) {
        return location.x + (windowWidth / 2) * zoom - (2 * levelR);
    } else {
        return location.x + (windowWidth / 2) * zoom;
    }
}

double Camera::TopEdge(double levelR) {
    double zoom = 1 / this->zoom;
    if(location.y - (windowHeight / 2) * zoom < -levelR) {
        return location.y - (windowHeight / 2) * zoom + (2 * levelR);
    } else {
        return location.y - (windowHeight / 2) * zoom;
    }
}

double Camera::BottomEdge(double levelR) {
    double zoom = 1 / this->zoom;
    if(location.y + (windowHeight / 2) * zoom > levelR) {
        return location.y + (windowHeight / 2) * zoom - (2 * levelR);
    } else {
        return location.y + (windowHeight / 2) * zoom;
    }
}
