#include "button.h"

Button::Button()
{
    x = 0;
    y = 0;
    defaultT = NULL;
    hoverT = NULL;
}

Button::Button(Texture *defaultT, Texture *hoverT, int x, int y, OnClickFunction OnClick) {
    this->x = x;
    this->y = y;
    this->defaultT = defaultT;
    this->hoverT = hoverT;
    this->OnClick = OnClick;
}

void Button::Render() {
    if(IsHovered()) {
        hoverT->Render(0, 0, x, y, 0);
    } else {
        defaultT->Render(0, 0, x, y, 0);
    }
}

bool Button::IsHovered() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if(mouseX < x)
        return false;
    if(mouseX > x + defaultT->Width())
        return false;
    if(mouseY < y)
        return false;
    if(mouseY < y - defaultT->Height())
        return false;

    return true;
}
