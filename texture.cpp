#include "texture.h"

Texture::Texture(){
    renderer   = NULL;
    rawTexture = NULL;
    width  = 0;
    height = 0;
}

Texture::Texture(SDL_Renderer *renderer, std::string path)
{
    // Sprawdzenie, czy przekazano prawidłowy renderer
    if (renderer == NULL) {
        std::cerr << "Texture::Texture Błąd, nie przekazano prawidłowego renderera." << std::endl;
        return;
    }
    // Przypisanie renderera
    this->renderer = renderer;

    // Ładowanie tymczasowej powierzchni, z której powstanie tekstura
    SDL_Surface* surface = IMG_Load(path.c_str());

    // Sprawdzenie, czy udało się załadować powierzchnię
    if(surface == NULL) {
        std::cerr << "Texture::Texture Błąd, nie udało się załadować obrazu ze śnieżki " << path << ". Błąd SDL_image: " << IMG_GetError() << std::endl;
        return;
    }

    // Ustalenie wymiarów obrazu
    width  = surface->w;
    height = surface->h;

    // Utworzenie tekstury z powierzchni
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);

    // Sprawdzenie, czy udało się utworzyć teksturę
    if (newTexture == NULL) {
        std::cerr << "Texture::Texture Błąd, nie udało się utworzyć tekstury z pliku " << path << ". Błąd SDL_image: " << IMG_GetError() << std::endl;
        return;
    }

    // Usuniêcie tymczasowej powierzchni z pamięci
    SDL_FreeSurface(surface);

    rawTexture = newTexture;

    this->path = path;
}

Texture::Texture(SDL_Renderer *renderer, std::__cxx11::string text, TTF_Font *font, SDL_Color textColor) {
    // Renderowanie powierzchni z tekstem
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
    if(surface == NULL) {
        std::cerr << "Texture::Texture Błąd, nie udało się utworzyć powierzchni z tekstem " << text << ". Błąd SDL_ttf: " << TTF_GetError() << std::endl;
        return;
    }

    // Przypisanie renderera
    this->renderer = renderer;

    // Ustalenie wymiarów obrazu
    width  = surface->w;
    height = surface->h;

    // Utworzenie tekstury z powierzchni
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);

    // Sprawdzenie, czy udało się utworzyć teksturę
    if (newTexture == NULL) {
        std::cerr << "Texture::Texture Błąd, nie udało się utworzyć tekstury z tekstem " << text << ". Błąd SDL_ttf: " << TTF_GetError() << std::endl;
        return;
    }

    // Usuniêcie tymczasowej powierzchni z pamięci
    SDL_FreeSurface(surface);

    rawTexture = newTexture;
}

void Texture::Render(Sint16 srcX, Sint16 srcY, Uint16 srcWidth, Uint16 srcHeight, Sint16 destX, Sint16 destY, Uint16 destWidth, Uint16 destHeight, double rotation, SDL_RendererFlip flip) {
    SDL_Rect source = { srcX, srcY, srcWidth, srcHeight };
    SDL_Rect destination = { destX, destY, destWidth, destHeight };
    SDL_RenderCopyEx(renderer, rawTexture, &source, &destination, rotation, NULL, flip);
}

void Texture::Render(Sint16 srcX, Sint16 srcY, Uint16 srcWidth, Uint16 srcHeight, Sint16 destX, Sint16 destY, double rotation, SDL_RendererFlip flip) {
    Render(srcX, srcY, srcWidth, srcHeight, destX, destY, srcWidth, srcHeight, rotation, flip);
}

void Texture::Render(Sint16 srcX, Sint16 srcY, Uint16 srcWidth, Uint16 srcHeight, Sint16 destX, Sint16 destY, double rotation) {
    Render(srcX, srcY, srcWidth, srcHeight, destX, destY, rotation, SDL_FLIP_NONE);
}

void Texture::Render(Sint16 srcX, Sint16 srcY, Sint16 destX, Sint16 destY, double rotation) {
    Render(srcX, srcY, width, height, destX, destY, rotation);
}

Uint16 Texture::Height() {
    return height;
}

Uint16 Texture::Width() {
    return width;
}

SDL_Renderer* Texture::Renderer() {
    return renderer;
}
