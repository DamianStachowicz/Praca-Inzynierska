#include "vector2d.h"

vector2d::vector2d()
{
    x = 0;
    y = 0;
}

vector2d::vector2d(double x, double y) {
    this->x = x;
    this->y = y;
}

vector2d::vector2d(std::__cxx11::string str) {
    std::string tmpStr = "";
    int i = 0;
    while(str[i] != ':') {
        tmpStr += str[i];
        ++i;
    }
    x = std::stod(tmpStr);
    tmpStr = str.erase(0, i+1);
    y = std::stod(tmpStr);
}

double vector2d::Length() {
    return sqrt(x*x + y*y);
}

vector2d vector2d::Direction() {
    return vector2d(x / Length(), y / Length());
}

vector2d vector2d::operator +(vector2d v) {
    return vector2d(this->x + v.x, this->y + v.y);;
}

vector2d vector2d::operator -(vector2d v){
    return vector2d(this->x - v.x, this->y - v.y);;
}

vector2d vector2d::operator *(double a) {
    return vector2d(x * a, y * a);
}

vector2d vector2d::operator /(double a) {
    return vector2d(x / a, y / a);
}

vector2d vector2d::operator -() {
    return vector2d(-x, -y);
}

double vector2d::operator *(vector2d v) {
    return x * v.x + y * v.y;
}

vector2d vector2d::ProjectTo(vector2d v) {
    return v * (vector2d(this->x, this->y) * v / v.Length());
}
