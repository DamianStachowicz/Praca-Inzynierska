#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include "texture.h"
#include "vector2d.h"
#include "camera.h"
#include "timer.h"
#include "animation.h"
#include "level.h"
#include "xmlhelper.h"

// typy obiektów w grze
enum {
    ENTITY_TYPE_UNDEFINED,
    ENTITY_TYPE_SPACESHIP,
    ENTITY_TYPE_PLANET,
    ENTITY_TYPE_ASTEROID,
    ENTITY_TYPE_ROCKET,
    ENTITY_TYPE_PARTICLE,
    ENTITY_TYPE_SELLING_POINT
};

class Entity
{
    protected:
        /* kołowe wykrywanie kolizji,
         * założono, że każda encja jest mniej-więcej okrągła,
         * założono, że środek geometryczny jest też środkiem masy
         */
        vector2d acceleration;
        vector2d force; // wypadkowa siła działająca na ciało
        Texture* texture;
        std::vector<Entity*> colliding; // wektor encji obecnie kolidujących z tą encją
        Uint32   deathAnimationStarted; // czas rozpoczęcia animacji zakończenia życia obiektu
        bool     alive;
        Animation animation;
        double maxSpeed;
        Uint8 state;

    public:
        static Timer timer;
        static std::vector<Entity*> entities;
        static Level level;
        vector2d location;
        double rotation;
        Uint8  type;
        double mass;
        vector2d velocity;
        double   r;     // promień
        vector2d collisionCenter;

        Entity();
        virtual ~Entity();
        virtual void Load(SDL_Renderer* renderer, std::string texturePath, Sint16 frameHeight, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass);
        virtual void Load(SDL_Renderer* renderer, std::string texturePath, Sint16 frameHeight, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass, vector2d collisionCenter, double r);
        virtual void Load(SDL_Renderer* renderer, std::string texturePath, Sint16 frameHeight, Sint16 frameWidth, Uint8 numberOfFrames,
                  Uint8 framesPerSecond, vector2d initLocation, double mass, vector2d collisionCenter, double r);
        void SetInitialVelocity(vector2d v);
        void Render();
        void Render(double xOffset, double yOffset);
        void RenderCopy(int position);
        bool CheckCollision(Entity* ent);
        vector2d AttractTo(Entity* ent);
        void UpdateAcceleration();
        void UpdateVelocity();
        void Move();
        void CheckCollisions();
        virtual void UpdateForce();
        virtual void Collide();
        virtual void Loop();
        bool Alive();
        bool DeathStarted();
        vector2d Momentum();
        void Bounce(Entity* ent);
        vector2d GetMinimapLocation(double ratio);
        double GetMinimapR(double ratio);
        virtual bool Serialize(std::ofstream& file);
        virtual bool Deserialize(std::ifstream& file, SDL_Renderer* renderer);
        vector2d CollisionCenter();
        Uint8 VisibleCopy();
};

#endif // ENTITY_H
