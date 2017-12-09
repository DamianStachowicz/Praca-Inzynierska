#ifndef LVLPICKSTATE_H
#define LVLPICKSTATE_H

#include <map>
#include <functional>
#include <menustate.h>
#include <tinyxml2.h>

struct lvl {
    std::string filename;
    bool        unlocked;
    Uint32      unlockScore;
    Uint32      bestScore;
};

class LvlPickState : public MenuState
{
    private:
        std::vector<lvl> levels;
        std::string lastLevel;
        std::map<std::string, std::string> btn2lvl;
        std::function<void()> Return2Menu;
        std::string selectedLevel;

        void LoadCfgFile();
        void RewriteCfgFile();

    public:
        LvlPickState();
        LvlPickState(SDL_Renderer* renderer, Uint32 windowWidth, Uint32 windowHeight, Texture* background);

        void Init(std::function<void()> ChangeState, std::function<void()> Return2Menu);
        std::string Filename();
        void HandleKeyDown(); // obsługa wciśnięcia klawisza na klawiaturze
        std::string LastLevel();
        void UpdateBestScore(Uint32 score);
        std::string NextLevel(std::string filename);
};

#endif // LVLPICKSTATE_H
