#include <iostream>
#include <SDL.h>
#include <string>

std::string getResourcePath(std::string str)
{
    std::string data_path;
    char *base_path = SDL_GetBasePath();
    if (base_path)
    {
        data_path = base_path;
    }
    else
    {
        data_path = SDL_strdup("./");
    }
    return data_path + "../../res/" + str + "/";
}

int main(int, char **)
{
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *tex = nullptr;
    int ret = -1;
    std::string imagePath = getResourcePath("Lesson1") + "hello.bmp";

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ret;
    }

    win = SDL_CreateWindow("Hello World!",
                           100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        goto exit;
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        goto exit;
    }

    std::cout << imagePath << std::endl;
    surface = SDL_LoadBMP(imagePath.c_str());
    if (surface == nullptr)
    {
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        goto exit;
    }

    tex = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    if (tex == nullptr)
    {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        goto exit;
    }

    for (int i = 0; i < 3; ++i)
    {
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        SDL_Delay(1000);
    }
    ret = 0;
exit:
    if (win)
    {
        SDL_DestroyWindow(win);
    }
    if (ren)
    {
        SDL_DestroyRenderer(ren);
    }
    if (tex)
    {
        SDL_DestroyTexture(tex);
    }

    SDL_Quit();
    return ret;
}