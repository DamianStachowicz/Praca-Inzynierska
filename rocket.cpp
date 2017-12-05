#include "rocket.h"

Rocket::Rocket() : Entity()
{
    type = ENTITY_TYPE_ROCKET;
    blastRadius = 50;
    lifeSpan = 1000;
    startTime = SDL_GetTicks();
}

Rocket::Rocket(Uint32 lifeSpan) : Entity() {
    type = ENTITY_TYPE_ROCKET;
    blastRadius = 50;
    this->lifeSpan = lifeSpan;
    startTime = SDL_GetTicks();
}

void Rocket::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        if(colliding[i]->type != ENTITY_TYPE_SPACESHIP) {
            Explode();
            break;
        }
    }
    colliding.clear();
}

void Rocket::Explode() {
    /* rakieta rozpada się na cząstki o określonej długości życia,
     * które mogą zderzyć się z innymi obiektami
     */
    uint numberOfParticles = 30;
    double particleMass = mass / numberOfParticles;
    double angle = 2*M_PI / numberOfParticles;
    double initVelocity = 0.5;
    for(uint i = 0; i < numberOfParticles; i++) {
        vector2d unitVector = vector2d(cos(i*angle), sin(i*angle));
        Particle* tmp = new Particle();
        tmp->Load(texture->Renderer(), "gfx/particle.png", 8, 16, 32, location + unitVector / 10, particleMass, vector2d(12, 0), 4);
        tmp->SetInitialVelocity(velocity + unitVector * initVelocity);
        tmp->rotation = 360 / numberOfParticles * i;
        entities.push_back(tmp);
    }
    alive = false;
}

void Rocket::Loop()
{
    Entity::Loop();
    if(deathAnimationStarted != 0 && SDL_GetTicks() - deathAnimationStarted > 100) {
        alive = false;
    }
    // jeżeli upłynął wyznaczony czas, rakieta eksploduje
    if(SDL_GetTicks() > startTime + lifeSpan) {
        Explode();
    }
}

bool Rocket::Deserialize(tinyxml2::XMLNode *root, SDL_Renderer *renderer) {
    return Entity::Deserialize(root, renderer);
}
