#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <vector2d.h>
#include "xmlhelper.h"

class Camera
{
    public:
        static Camera camera;
        vector2d location;
        double zoom;
        Uint32 windowWidth;
        Uint32 windowHeight;

        Camera();
        void Move(vector2d shift);
        void SetSize(Uint32 width, Uint32 height);
        bool Serialize(std::ofstream& file);
        bool Deserialize(std::ifstream& file);
        double LeftEdge(double levelR);
        double RightEdge(double levelR);
        double TopEdge(double levelR);
        double BottomEdge(double levelR);
};

#endif // CAMERA_H
