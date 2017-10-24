#include "xmlhelper.h"

XMLhelper::XMLhelper()
{

}

std::string XMLhelper::GetValue(std::ifstream& file, std::string tag) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby odczytania tagu. Plik nie jest otwarty do odczytu." << std::endl;
        return "error";
    }

    std::string str = "";
    char ch;
    for(uint i = 0; i < tag.length(); ++i) {
        file.get();
    }
    while( ( ch = file.get() ) != '<') {
        str += ch;
    }

    // tworzenie tagu zamykającego
    for(uint i = 0; i < tag.length(); ++i) {
        file.get();
    }

    return str;
}

void XMLhelper::SkipTag(std::ifstream &file, std::__cxx11::string tag) {
    if(!file.is_open()) {
        std::cerr << "Błąd podczas próby ominięcia tagu. Plik nie jest otwarty do odczytu." << std::endl;
        return;
    }

    for(uint i = 0; i < tag.length(); ++i) {
        file.get();
    }
}
