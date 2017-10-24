#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <fstream>
#include <iostream>

class XMLhelper
{
    public:
        XMLhelper();
        static std::string GetValue(std::ifstream& file, std::string tag);
        static void SkipTag(std::ifstream& file, std::string tag);
};

#endif // XMLHELPER_H
