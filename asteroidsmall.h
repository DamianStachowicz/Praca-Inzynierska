#ifndef ASTEROIDSMALL_H
#define ASTEROIDSMALL_H

#include "asteroid.h"

class AsteroidSmall : public Asteroid
{
    public:
        AsteroidSmall();
        void Collide();
};

#endif // ASTEROIDSMALL_H
