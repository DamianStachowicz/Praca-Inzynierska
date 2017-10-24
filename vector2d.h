#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>
#include <iostream>

class vector2d
{ 
    public:
        double x;
        double y;

        vector2d();
        vector2d(double x, double y);
        vector2d(std::string str);
        double Length();
        vector2d Direction();
        vector2d operator +(vector2d v);
        vector2d operator -(vector2d v);
        vector2d operator *(double a);
        vector2d operator /(double a);
        vector2d operator -();
        double operator *(vector2d v);
        vector2d ProjectTo(vector2d v);
};
#endif // VECTOR2D_H
