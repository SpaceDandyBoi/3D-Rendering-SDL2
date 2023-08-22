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
struct polygon{vec3D points[3];};
struct triangle{vec2D points[3];};
struct mesh{std::vector<polygon> polygons;};
struct Rotation{float xRotation, yRotation, zRotation;};
struct mat4x4
{
    float m[4][4] = { 0 };
};

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
    void rotateX(vec3D &point, vec3D &pointr);
    void rotateY(vec3D &point, vec3D &pointr);
    void rotateZ(vec3D &point, vec3D &pointr);
    void projection(vec3D &point,vec2D &pointr);

    Rotation rotation = {0,0,0};
    float FOV = 100.0f;
    float scale = 30.0f;
    int WindowSizeX;
    int WindowSizeY;
    float DeltaTime = 0.0f;



    SDL_Renderer* renderer;
    vec2D pos;

    mesh meshObject;
    vec3D vCamera;
    mat4x4 matProj;

    void MultiplyMatrixVector(vec3D &i, vec3D &o, mat4x4 &m)
    {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w; o.y /= w; o.z /= w;
        }
    }


};
