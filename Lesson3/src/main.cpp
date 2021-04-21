#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_SIZE = 40;

void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr)
    {
        logSDLError(std::cout, "IMG_LoadTexture");
    }
    return texture;
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

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

int main(int, char **)
{
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture *background = nullptr;
    SDL_Texture *image = nullptr;
    int ret = -1;
    int xTiles, yTiles;
    int iW, iH;
    int x, y;
    std::string resPath = getResourcePath("Lesson3");
    std::cout << resPath << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ret;
    }

    win = SDL_CreateWindow("Lesson 3",
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

    background = loadTexture(resPath + "background.png", ren);
    image = loadTexture(resPath + "image.png", ren);
    if (background == nullptr || image == nullptr)
    {
        goto exit;
    }

    xTiles = SCREEN_WIDTH / TILE_SIZE;
    yTiles = SCREEN_HEIGHT / TILE_SIZE;

    SDL_RenderClear(ren);
    //Draw the tiles by calculating their positions
    for (int i = 0; i < xTiles * yTiles; ++i)
    {
        x = i % xTiles;
        y = i / xTiles;
        renderTexture(background, ren, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }

    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    x = SCREEN_WIDTH / 2 - iW / 2;
    y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image, ren, x, y);

    SDL_RenderPresent(ren);
    SDL_Delay(2000);

    ret = 0;
exit:
    cleanup(win, ren, background, image);
    SDL_Quit();
    return ret;
}