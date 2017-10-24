#include "asteroidmiddle.h"

AsteroidMiddle::AsteroidMiddle()
{
    type = ENTITY_TYPE_ASTEROID;
    size = 2;
}

void AsteroidMiddle::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        switch(colliding[i]->type) {
            case ENTITY_TYPE_ROCKET:
                if( !(colliding[i]->DeathStarted()) ) {
                    alive = false;

                    vector2d momentum = colliding[i]->Momentum();

                    AsteroidSmall* a1 = new AsteroidSmall();
                    AsteroidSmall* a2 = new AsteroidSmall();

                    Entity::entities.push_back(a1);
                    a1->Load(texture->Renderer(), "gfx/Asteroid-Small.png", 43, 1, 1, location + (location / location.Length()) * r, mass / 2);
                    a1->SetInitialVelocity(momentum / mass + velocity);

                    Entity::entities.push_back(a2);
                    a2->Load(texture->Renderer(), "gfx/Asteroid-Small.png", 43, 1, 1, location - (location / location.Length()) * r, mass / 2);
                    a2->SetInitialVelocity(momentum / mass + velocity);
                } break;
            case ENTITY_TYPE_PARTICLE: Asteroid::ParticleCollide(colliding[i]); break;
            case ENTITY_TYPE_SELLING_POINT: alive = false; break;
            default: Bounce(colliding[i]); break;
        }
    }
    colliding.clear();
}
