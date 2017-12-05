#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SDL2/SDL.h>
#include "entity.h"
#include "rocket.h"

class SpaceShip: public Entity
{
    private:
        Sint8 health;
        double jet;

    public:
        bool advance;
        bool stop;
        Rocket* rocket;

        SpaceShip();
        void Collide();
        void UpdateForce();
        void Rotate(double angle);
        void Shoot(SDL_Renderer* renderer, vector2d initVelocity);
        Sint8 Health();
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file, SDL_Renderer* renderer);
        bool Deserialize(tinyxml2::XMLElement* root, SDL_Renderer* renderer);
        void DecreaseHealth(double amount);
        Uint8 VisibleCopy();
};

#endif // SPACESHIP_H
