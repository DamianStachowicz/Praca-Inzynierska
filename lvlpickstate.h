#ifndef LVLPICKSTATE_H
#define LVLPICKSTATE_H

#include <map>
#include <functional>
#include <menustate.h>
#include <tinyxml2.h>

class LvlPickState : public MenuState
{
    private:
        std::map<std::string, bool> levels;
        std::map<std::string, std::string> btn2lvl;
        std::function<void()> Return2Menu;
        std::string lastLevel;

    public:
        LvlPickState();
        LvlPickState(SDL_Renderer* renderer, Uint32 windowWidth, Uint32 windowHeight, Texture* background);

        void Init(std::function<void()> ChangeState, std::function<void()> Return2Menu);
        std::string Filename();
        void HandleKeyDown(); // obsługa wciśnięcia klawisza na klawiaturze
        std::string LastLevel();
};

#endif // LVLPICKSTATE_H
