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

void LvlPickState::LoadCfgFile() {
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile("cfg/levelList.xml");
    tinyxml2::XMLNode* root = xmlDoc.FirstChild();
    tinyxml2::XMLElement* element = root->FirstChildElement();
    tinyxml2::XMLElement* subelement;
    lvl tmpLvl;
    while(element != NULL) {
        subelement = element->FirstChildElement("filename");
        tmpLvl.filename = subelement->GetText();

        subelement = element->FirstChildElement("unlocked");
        subelement->QueryBoolText(&tmpLvl.unlocked);

        subelement = element->FirstChildElement("unlockScore");
        subelement->QueryUnsignedText(&tmpLvl.unlockScore);

        subelement = element->FirstChildElement("bestScore");
        subelement->QueryUnsignedText(&tmpLvl.bestScore);

        levels.push_back(tmpLvl);

        element = element->NextSiblingElement();
    }

    Texture* defaultBtn = new Texture(renderer, "gfx/buttonBlue.png");
    Texture* selectedBtn = new Texture(renderer, "gfx/buttonRed.png");
    Button* btn;
    std::string btnLabel;
    for(uint i = 0; i < levels.size(); i++) {
        if(levels[i].unlocked) {
            btnLabel = std::to_string(i+1) + ". wynik: " + std::to_string(levels[i].bestScore);
            btn = new Button(renderer, defaultBtn, selectedBtn, TTF_OpenFont( "ttf/kenvector_future.ttf", 20 ),
                             btnLabel, {0x0, 0x0, 0x0, 0xFF},
                             windowWidth / 2 - 111, 20 + 60 * i, ChangeState);
            lastLevel = btnLabel;
            btn2lvl[btnLabel] = levels[i].filename;
        } else {
            btnLabel = "zablokowany";
            btn = new Button(renderer, defaultBtn, selectedBtn, TTF_OpenFont( "ttf/kenvector_future.ttf", 20 ),
                             btnLabel, {0x0, 0x0, 0x0, 0xFF}, windowWidth / 2 - 111, 20 + 60 * i, ChangeState);
        }
        AddButton(btn);
    }
    buttons[0]->Switch();
    selectedLevel = lastLevel;
}

void LvlPickState::Init(std::function<void()> ChangeState, std::function<void()> Return2Menu) {
    this->ChangeState = ChangeState;
    this->Return2Menu = Return2Menu;
    LoadCfgFile();
}

std::string LvlPickState::Filename() {
    std::string text = buttons[currentBtnIdx]->text;
    selectedLevel = text;
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

std::string LvlPickState::LastLevel() {
    selectedLevel = lastLevel;
    return btn2lvl[lastLevel];
}

void LvlPickState::UpdateBestScore(Uint32 score) {
    for(uint i = 0; i < levels.size(); i++) {
        if(levels[i].filename == btn2lvl[selectedLevel] && levels[i].bestScore <= score) {
            levels[i].bestScore = score;
            if(levels[i+1].unlockScore <= score) {
                levels[i+1].unlocked = true;
            }
            break;
        }
    }

    RewriteCfgFile();
    levels.clear();
    LoadCfgFile();
}

void LvlPickState::RewriteCfgFile() {
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLElement* root = xmlDoc.NewElement("LevelList");
    xmlDoc.InsertFirstChild(root);

    tinyxml2::XMLElement* element;
    tinyxml2::XMLElement* subelement;
    for(uint i = 0; i < levels.size(); i++) {
        element = xmlDoc.NewElement("Level");
        root->InsertEndChild(element);

        subelement = xmlDoc.NewElement("filename");
        subelement->SetText(levels[i].filename.c_str());
        element->InsertEndChild(subelement);

        subelement = xmlDoc.NewElement("unlocked");
        subelement->SetText(levels[i].unlocked);
        element->InsertEndChild(subelement);

        subelement = xmlDoc.NewElement("unlockScore");
        subelement->SetText(levels[i].unlockScore);
        element->InsertEndChild(subelement);

        subelement = xmlDoc.NewElement("bestScore");
        subelement->SetText(levels[i].bestScore);
        element->InsertEndChild(subelement);
    }

    tinyxml2::XMLError result = xmlDoc.SaveFile("cfg/levelList.xml");
    if(result != tinyxml2::XML_SUCCESS) {
        std::cerr << "Nie udało się zakualizować pliku levelList.xml\n";
    }
}
