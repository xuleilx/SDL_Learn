#include <iostream>
#include <SDL.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr)
    {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        if (texture == nullptr)
        {
            logSDLError(std::cout, "SDL_CreateTextureFromSurface");
        }
    }
    else
    {
        logSDLError(std::cout, "SDL_LoadBMP");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int, char **)
{
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture *background = nullptr;
    SDL_Texture *image = nullptr;
    int ret = -1;
    int bW, bH;
    int iW, iH;
    int x, y;
    std::string resPath = getResourcePath("Lesson2");
    std::cout<< resPath <<std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return ret;
    }

    win = SDL_CreateWindow("Lesson 2",
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

    background = loadTexture(resPath + "background.bmp", ren);
    image = loadTexture(resPath + "image.bmp", ren);
    if (background == nullptr || image == nullptr)
    {
        goto exit;
    }

    for (int i = 0; i < 3; ++i)
    {
        SDL_RenderClear(ren);
        SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
        renderTexture(background, ren, 0, 0);
        renderTexture(background, ren, bW, 0);
        renderTexture(background, ren, 0, bH);
        renderTexture(background, ren, bW, bH);

        SDL_QueryTexture(image,NULL,NULL,&iW,&iH);
        x = SCREEN_WIDTH / 2 - iW / 2;
        y = SCREEN_HEIGHT / 2 -iH / 2;
        renderTexture(image,ren,x,y);
        
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
    if (background)
    {
        SDL_DestroyTexture(background);
    }
    if (image)
    {
        SDL_DestroyTexture(image);
    }

    SDL_Quit();
    return ret;
}