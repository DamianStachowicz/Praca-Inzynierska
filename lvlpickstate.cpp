#include "lvlpickstate.h"

LvlPickState::LvlPickState()
{
    MenuState();
}

LvlPickState::LvlPickState(SDL_Renderer *renderer, Uint32 windowWidth, Uint32 windowHeight, Texture *background) {
    this->renderer = renderer;
    this->windowHeight = windowHeight;
    this->windowWidth = windowWidth;
    this->background = background;
    currentBtnIdx = 0;
}

void LvlPickState::Init(std::function<void()> ChangeState, std::function<void()> Return2Menu) {
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile("cfg/levelList.xml");
    tinyxml2::XMLNode* root = xmlDoc.FirstChild();
    tinyxml2::XMLElement* element = root->FirstChildElement();
    tinyxml2::XMLElement* subelement;
    std::string filename;
    bool unlocked;
    while(element != NULL) {
        subelement = element->FirstChildElement("fileName");
        filename = subelement->GetText();
        subelement = element->FirstChildElement("unlocked");
        subelement->QueryBoolText(&unlocked);
        levels[filename] = unlocked;
        element = element->NextSiblingElement();
    }

    this->ChangeState = ChangeState;
    this->Return2Menu = Return2Menu;

    int i = 1;
    Texture* defaultBtn = new Texture(renderer, "gfx/buttonBlue.png");
    Texture* selectedBtn = new Texture(renderer, "gfx/buttonRed.png");
    Button* btn;
    for(auto const& x : levels) {
        if(x.second) {
            btn = new Button(renderer, defaultBtn, selectedBtn, TTF_OpenFont( "ttf/kenvector_future.ttf", 40 ),
                             std::to_string(i), {0x0, 0x0, 0x0, 0xFF}, windowWidth / 2 - 111, 20 + 60 * (i - 1), ChangeState);
        } else {
            btn = new Button(renderer, defaultBtn, selectedBtn, TTF_OpenFont( "ttf/kenvector_future.ttf", 20 ),
                             "zablokowany", {0x0, 0x0, 0x0, 0xFF}, windowWidth / 2 - 111, 20 + 60 * (i - 1), ChangeState);
        }
        AddButton(btn);
        btn2lvl[std::to_string(i)] = x.first;
        ++i;
    }
    buttons[0]->Switch();
}

std::string LvlPickState::Filename() {
    std::string text = buttons[currentBtnIdx]->text;
    return btn2lvl[text];
}

void LvlPickState::HandleKeyDown() {
    if( keyStates[ SDL_SCANCODE_S ] || keyStates[ SDL_SCANCODE_DOWN ] ) {
        ButtonDown();
    } else if( keyStates[ SDL_SCANCODE_W ] || keyStates[ SDL_SCANCODE_UP ] ) {
        ButtonUp();
    } else if( keyStates[ SDL_SCANCODE_RETURN ] ) {
        buttons[ currentBtnIdx ]->OnClick();
    } else if( keyStates[ SDL_SCANCODE_ESCAPE ] ) {
        Return2Menu();
    }
}
