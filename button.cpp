#include "button.h"

Button::Button(SDL_Renderer *renderer, Texture *defaultBg, Texture *selectedBg, TTF_Font *font,
               std::__cxx11::string text, SDL_Color color, int x, int y) {
    this->renderer = renderer;
    this->defaultBg = defaultBg;
    this->selectedBg = selectedBg;
    this->font = font;
    this->text = text;
    selected = false;
    this->x = x;
    this->y = y;
    textTexture = new Texture(renderer, text, font, color);
    this->OnClick = OnClick;
}

void Button::Render() {
    if(selected) {
        selectedBg->Render(0, 0, x, y, 0);
    } else {
        defaultBg->Render(0, 0, x, y, 0);
    }
    textTexture->Render(0, 0, x + defaultBg->Width() / 2 - textTexture->Width() / 2, y, 0);
}

void Button::Switch() {
    selected = !selected;
}
