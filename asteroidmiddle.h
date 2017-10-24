#ifndef ASTEROIDMIDDLE_H
#define ASTEROIDMIDDLE_H

#include "asteroid.h"
#include "asteroidsmall.h"

class AsteroidMiddle : public Asteroid
{
    public:
        AsteroidMiddle();
        void Collide();
};

#endif // ASTEROIDMIDDLE_H
