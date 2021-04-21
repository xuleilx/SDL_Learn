#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_SIZE = 40;

void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr)
{
    SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr)
    {
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else
    {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    renderTexture(tex, ren, dst, clip);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture *renderText(const std::string &message, const std::string &fontFile,
                        SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr)
    {
        logSDLError(std::cout, "TTF_OpenFont");
        return nullptr;
    }

    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr)
    {
        TTF_CloseFont(font);
        logSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr)
    {
        logSDLError(std::cout, "CreateTexture");
    }

    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

int main(int, char **)
{
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture *image = nullptr;
    int ret = -1;
    int iW, iH;
    int x, y;
    SDL_Color color = {255, 255, 255, 255};
    std::string resPath = getResourcePath("Lesson6");
    std::cout << resPath << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ret;
    }

    if (TTF_Init() != 0)
    {
        logSDLError(std::cout, "TTF_Init");
        SDL_Quit();
        return 1;
    }
    win = SDL_CreateWindow("Lesson 6",
                           100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        logSDLError(std::cout, "SDL_CreateWindow");
        goto exit;
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr)
    {
        logSDLError(std::cout, "SDL_CreateRenderer");
        goto exit;
    }

    image = renderText("TTF fonts are cool!", resPath + "sample.ttf",
                       color, 64, ren);
    if (image == nullptr)
    {
        goto exit;
    }

    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    x = SCREEN_WIDTH / 2 - iW / 2;
    y = SCREEN_HEIGHT / 2 - iH / 2;
    SDL_RenderClear(ren);

    renderTexture(image, ren, x, y);
    SDL_RenderPresent(ren);
    SDL_Delay(2000);
    ret = 0;
exit:
    cleanup(win, ren, image);
    SDL_Quit();
    TTF_Quit();
    return ret;
}