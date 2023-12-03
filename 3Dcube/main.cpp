#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "renderer3d.h"


mesh cube;

polygon polycreator(Point3D Apoint, Point3D Bpoint, Point3D Cpoint){
    return polygon {{Apoint,Bpoint,
                  Bpoint,Cpoint,
                  Cpoint,Apoint}};

}

int main(int argc, char** argv) {
    cube = {{{polycreator(Point3D(-1.0f,-1.0f,-1.0f),Point3D(-1.0f,1.0f,-1.0f),Point3D(1.0f,1.0f,-1.0f))},
             {polycreator(Point3D(-1.0f,-1.0f,-1.0f),Point3D(1.0f,1.0f,-1.0f),Point3D(1.0f,-1.0f,-1.0f))},

             {polycreator(Point3D(1.0f,-1.0f,-1.0f),Point3D(1.0f,1.0f,-1.0f),Point3D(1.0f,1.0f,1.0f))},
             {polycreator(Point3D(1.0f,-1.0f,-1.0f),Point3D(1.0f,1.0f,1.0f),Point3D(1.0f,-1.0f,1.0f))},

             {polycreator(Point3D(1.0f, -1.0f, 1.0f),Point3D(1.0f, 1.0f, 1.0f),Point3D(-1.0f, 1.0f, 1.0f))},
             {polycreator(Point3D(1.0f, -1.0f, 1.0f),Point3D(-1.0f, 1.0f, 1.0f),Point3D(-1.0f, -1.0f, 1.0f))},

             {polycreator(Point3D( -1.0f, -1.0f, 1.0f),Point3D(-1.0f, 1.0f, 1.0f),Point3D(-1.0f, 1.0f, -1.0f))},
             {polycreator(Point3D(-1.0f, -1.0f, 1.0f),Point3D(-1.0f, 1.0f, -1.0f),Point3D(-1.0f, -1.0f, -1.0f))},

             {polycreator(Point3D(-1.0f, 1.0f, -1.0f),Point3D(-1.0f, 1.0f, 1.0f),Point3D(1.0f, 1.0f, 1.0f))},
             {polycreator(Point3D(-1.0f, 1.0f, -1.0f),Point3D(1.0f, 1.0f, 1.0f),Point3D(1.0f, 1.0f, -1.0f))},

             {polycreator(Point3D(1.0f, -1.0f, 1.0f),Point3D(-1.0f, -1.0f, 1.0f),Point3D(-1.0f, -1.0f, -1.0f))},
             {polycreator(Point3D(1.0f, -1.0f, 1.0f),Point3D(-1.0f, -1.0f, -1.0f),Point3D(1.0f, -1.0f, -1.0f))}}};

    //creating sdl window and renderer
    SDL_Window* window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 540,
                              0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //create 3D renderer
    renderer3D renderer3d(cube,window,renderer);

    //engine3D engines3D(window,renderer,meshCube);

    //framerate
    const int FPS = 144;
    const int frameDelay = 1000 /FPS;
    Uint32 frameStart;
    int frameTime;
    bool running = true;

    const Uint8 *keyState;
    SDL_Event e;
    //loop
    while (running){
        frameStart = SDL_GetTicks();
        float dirx, diry;
        
        while (SDL_PollEvent(&e)){
            switch (e.type) {
                default:
                    break;
            }
            if (SDL_QuitRequested()){
                running = false;
                break;
            }
        }

        //MOVEMENT
        keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_DOWN]) dirx = 0.03f;
        else if(keyState[SDL_SCANCODE_UP]) dirx = -0.03f;
        if (keyState[SDL_SCANCODE_RIGHT]) diry = 0.03f;
        else if (keyState[SDL_SCANCODE_LEFT]) diry = -0.03f;
        if (keyState[SDL_SCANCODE_W])renderer3d.setPos(Point2D(renderer3d.getPos().x,renderer3d.getPos().y - 4.0f));
        else if (keyState[SDL_SCANCODE_S])renderer3d.setPos(Point2D(renderer3d.getPos().x,renderer3d.getPos().y + 4.0f));
        if (keyState[SDL_SCANCODE_A])renderer3d.setPos(Point2D(renderer3d.getPos().x - 4.0f,renderer3d.getPos().y));
        else if (keyState[SDL_SCANCODE_D])renderer3d.setPos(Point2D(renderer3d.getPos().x + 4.0f,renderer3d.getPos().y));
        renderer3d.setRotation(Rotation{renderer3d.getRotation().xRotation + dirx, renderer3d.getRotation().yRotation + diry});

        diry = 0, dirx = 0;

        //int mx, my;
        //Uint32 buttons = SDL_GetMouseState(&mx, &my);
        //std::cout << "Mouse cursor is at: " << mx << " " << my << std::endl;
        //renderer3d.setRotationX(mx);
        //renderer3d.setRotationY(my);
        //render
        renderer3d.render();


        //frameRate
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }


    SDL_Quit();
    return 0;
}
