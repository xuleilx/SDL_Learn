#include<iostream>
#include<SDL.h>

void InitializeDataPath(char **data_path) {
    char *base_path = SDL_GetBasePath();
    if (base_path) {
        *data_path = base_path;
    } else {
        *data_path = SDL_strdup("./");
    }
}

int main(int,char**){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
        return 1;
    }
    char *data_path = NULL;
    InitializeDataPath(&data_path);
    std::cout<<"data_path: "<<data_path<<std::endl;
    SDL_Quit();
    return 0;
}