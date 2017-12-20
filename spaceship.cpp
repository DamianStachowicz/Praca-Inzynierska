#include "spaceship.h"

SpaceShip::SpaceShip() : Entity()
{
    type = ENTITY_TYPE_SPACESHIP;
    health = 100;
    jet = 1;
    rocket = NULL;
}

void SpaceShip::Collide() {
    for(uint i = 0; i < colliding.size(); i++) {
        Entity *ent = colliding[i];
        switch (ent->type) {
            //case ENTITY_TYPE_PLANET: Bounce(ent); DecreaseHealth(20); break;
            case ENTITY_TYPE_ROCKET: continue;
            case ENTITY_TYPE_SELLING_POINT: break;
            case ENTITY_TYPE_PARTICLE: continue;
            default: Bounce(ent); DecreaseHealth(10); break;
        }
        double sum = r + ent->r;
        vector2d direction = (location - ent->location).Direction();
        location = ent->location + direction * (sum + 2);
    }
    colliding.clear();
}

void SpaceShip::DecreaseHealth(double amount) {
    health -= amount;
    state = ( 100 - health ) / 20;
}

void SpaceShip::UpdateForce() {
    if(stop) {
        velocity = velocity * 0.995;
    }
    Entity::UpdateForce();
    // chwilowo potrzebna jest rotacja w radianach, a nie w stopniach
    double rotation = this->rotation * M_PI / 180.0;
    vector2d direction = vector2d(cos(rotation), sin(rotation));
    if(advance) {
        force = force + ( direction * jet );
    }
}

void SpaceShip::Rotate(double angle) {
    // Kąt podany jest w radianach, a rotacja jest w stopniach
    rotation = angle * 180 / M_PI;
}

void SpaceShip::Shoot(SDL_Renderer* renderer, vector2d initVelocity) {
    rocket = new Rocket();
    entities.push_back(rocket);
    vector2d location = this->location + vector2d(cos(rotation), sin(rotation));
    rocket->Load(renderer, "gfx/rocket.png", 16, 2, 16, location, 10000, vector2d(17, 0), 8);
    rocket->SetInitialVelocity(velocity + initVelocity);
    rocket->rotation = this->rotation;
}

Sint8 SpaceShip::Health() {
    return health;
}

bool SpaceShip::Deserialize(tinyxml2::XMLElement *root, SDL_Renderer *renderer) {
    return Entity::Deserialize(root, renderer);;
}

Uint8 SpaceShip::VisibleCopy() {
    return LEVEL_COPY_DEFAULT;
}
