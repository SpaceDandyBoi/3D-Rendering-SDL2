//
// Created by Dandy on 7/26/2023.
//

#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <iostream>

struct Point2D {float x, y; };
struct Point3D {float x, y, z;};
struct vertex {Point3D start, end;};
struct polygon{vertex poly[3];};
struct mesh{std::vector<polygon> pmesh;};
struct Rotation{float xRotation, yRotation;};


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

    const Point2D &getPos() const;

    void setPos(const Point2D &pos);

    const Rotation &getRotation() const;

    void setRotation(const Rotation &rotation);

private:
    Point3D rotateX(Point3D point);
    Point3D rotateY(Point3D point);
    Point2D projection(Point3D point);

    Rotation rotation;
    float FOV = 10.0f;
    float scale = 100;

    float DeltaTime = 0.0f;

    int WindowSizeX;
    int WindowSizeY;
    SDL_Renderer* renderer;
    Point2D pos;

    mesh meshObject;
};