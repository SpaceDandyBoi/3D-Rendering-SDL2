#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "renderer3d.h"


mesh cube;

/*
polygon polycreator(vec3D Apoint, vec3D Bpoint, vec3D Cpoint){
    return polygon {{Apoint,Bpoint,
                  Bpoint,Cpoint,
                  Cpoint,Apoint}};

}
 */

int main(int argc, char** argv) {
/*
    cube = {{		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
                     { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

                    // EAST
                     { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
                     { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

                    // NORTH
                     { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
                     { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

                    // WEST
                     { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
                     { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

                    // TOP
                     { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
                     { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

                    // BOTTOM
                     { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
                     { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
                     }};
*/

    cube.LoadFromObjectFile("filename.obj");
    //creating sdl window and renderer
    SDL_Window* window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 720,
                              0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, 240,180 );
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
        float dirx = 0, diry = 0, dirz = 0;
        
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
        if (keyState[SDL_SCANCODE_RIGHT]) diry = -0.03f;
        else if (keyState[SDL_SCANCODE_LEFT]) diry = 0.03f;
        if (keyState[SDL_SCANCODE_1]) dirz = -0.03f;
        else if (keyState[SDL_SCANCODE_2]) dirz = 0.03f;
        if (keyState[SDL_SCANCODE_W])renderer3d.setPos(vec2D(renderer3d.getPos().x,renderer3d.getPos().y - 4.0f));
        else if (keyState[SDL_SCANCODE_S])renderer3d.setPos(vec2D(renderer3d.getPos().x,renderer3d.getPos().y + 4.0f));
        if (keyState[SDL_SCANCODE_A])renderer3d.setPos(vec2D(renderer3d.getPos().x - 4.0f,renderer3d.getPos().y));
        else if (keyState[SDL_SCANCODE_D])renderer3d.setPos(vec2D(renderer3d.getPos().x + 4.0f,renderer3d.getPos().y));
        renderer3d.setRotation(Rotation{renderer3d.getRotation().xRotation + dirx,
                                        renderer3d.getRotation().yRotation + diry,
                                        renderer3d.getRotation().zRotation + dirz});

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
