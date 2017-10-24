#ifndef ASTEROIDBIG_H
#define ASTEROIDBIG_H

#include "asteroid.h"
#include "asteroidmiddle.h"
#include "rocket.h"

class AsteroidBig : public Asteroid
{
    public:
        AsteroidBig();
        void Collide();
};

#endif // ASTEROIDBIG_H
