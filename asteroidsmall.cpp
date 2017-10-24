#include "asteroidsmall.h"

AsteroidSmall::AsteroidSmall()
{
    type = ENTITY_TYPE_ASTEROID;
    size = 1;
}

void AsteroidSmall::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        switch(colliding[i]->type) {
            case ENTITY_TYPE_ROCKET: if( !(colliding[i]->DeathStarted()) ) alive = false; break;
            case ENTITY_TYPE_PARTICLE: Asteroid::ParticleCollide(colliding[i]); break;
            case ENTITY_TYPE_SELLING_POINT: alive = false; break;
            default: Bounce(colliding[i]); break;
        }
    }
    colliding.clear();
}
