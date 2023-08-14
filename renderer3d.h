//
// Created by Dandy on 7/26/2023.
//

#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <iostream>

struct vec2D {float x, y; };
struct vec3D {float x, y, z;};
struct line {vec3D start, end;};
struct polygon{
    vec3D points[3];
    line lines[3]{
            {vec3D {points[0].x,points[0].y,points[0].z},vec3D {points[1].x,points[1].y,points[1].z}},
            {vec3D {points[1].x,points[1].y,points[1].z},vec3D {points[2].x,points[2].y,points[2].z}},
            {vec3D {points[2].x,points[2].y,points[2].z},vec3D {points[0].x,points[0].y,points[0].z}}
    };
};
struct mesh{std::vector<polygon> polygons;};
struct Rotation{float xRotation, yRotation, zRotation;};


class renderer3D{
public:
    renderer3D(mesh P_meshObject, SDL_Window* window, SDL_Renderer* p_renderer);

    void render();

    void print(){
        std::cout << DeltaTime << std::endl;
    }

    float getDeltaTime() const;

    void setDeltaTime(float deltaTime);

    static int getWindowSizeX() ;

    void setWindowSizeX(int windowSizeX);

    static int getWindowSizeY() ;

    void setWindowSizeY(int windowSizeY);


    float getFov() const;

    void setFov(float fov);

    float getScale() const;

    void setScale(float scale);

    const vec2D &getPos() const;

    void setPos(const vec2D &pos);

    const Rotation &getRotation() const;

    void setRotation(const Rotation &rotation);

private:
    vec3D rotateX(vec3D point);
    vec3D rotateY(vec3D point);
    vec3D rotateZ(vec3D point);
    vec2D projection(vec3D point);

    Rotation rotation = {0,0,0};
    float FOV = 100.0f;
    float scale = 30.0f;

    float DeltaTime = 0.0f;

    int WindowSizeX;
    int WindowSizeY;
    SDL_Renderer* renderer;
    vec2D pos;

    mesh meshObject;
    vec3D vCamera;
};
