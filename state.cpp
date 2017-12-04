#include "state.h"

State::State() {}
void State::Init(std::function<void()> ChangeState) {}
void State::Loop() {}
void State::Render() {}
void State::HandleEvent(SDL_Event *event) {}
void State::HandleKeyDown() {}
void State::HandleKeyUp() {}
void State::HandleMouseButtonDown(SDL_MouseButtonEvent *event) {}
void State::HandleMouseMotion(SDL_MouseMotionEvent *event) {}
void State::HandleMouseWheelScroll(SDL_MouseWheelEvent *event) {}
