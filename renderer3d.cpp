//
// Created by Dandy on 7/26/2023.
//

#include "renderer3d.h"

#include <utility>
#include <unistd.h>
#define EPS 1e-9

renderer3D::renderer3D(mesh P_meshObject, SDL_Window* window, SDL_Renderer* p_renderer){
    //SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);
    meshObject = std::move(P_meshObject);
    renderer = p_renderer;
    SDL_RenderGetLogicalSize(renderer, &WindowSizeX, &WindowSizeY);
    pos = vec2D(WindowSizeX/2,WindowSizeY/2);
    rotation = {0,0};
}


//=================================================================================

void renderer3D::render() {
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    //rotation.xRotation += 1* DeltaTime;
    //rotation.yRotation += 1*DeltaTime;
    SDL_SetRenderDrawColor(renderer3D::renderer,255,255,255,SDL_ALPHA_OPAQUE);
    for (auto& poly : meshObject.polygons) {

        //rotate lines
        polygon triRotated;
        for (int i = 0; i < 3; ++i) {
            triRotated.lines[i].start = rotateX(rotateY(rotateZ(poly.lines[i].start)));
            triRotated.lines[i].end = rotateX(rotateY(rotateZ(poly.lines[i].end)));
        }

        //normals
        vec3D normal;
        normal.x = (triRotated.lines[0].end.y - triRotated.lines[0].start.y) * (triRotated.lines[2].start.z - triRotated.lines[2].end.z) -
                (triRotated.lines[0].end.z - triRotated.lines[0].start.z) * (triRotated.lines[2].start.y - triRotated.lines[2].end.y);
        normal.y = (triRotated.lines[0].end.z - triRotated.lines[0].start.z) * (triRotated.lines[2].start.x - triRotated.lines[2].end.x) -
                (triRotated.lines[0].end.x - triRotated.lines[0].start.x) * (triRotated.lines[2].start.z - triRotated.lines[2].end.z);
        normal.z = (triRotated.lines[0].end.x - triRotated.lines[0].start.x) * (triRotated.lines[2].start.y - triRotated.lines[2].end.y) -
                (triRotated.lines[0].end.y - triRotated.lines[0].start.y) * (triRotated.lines[2].start.x - triRotated.lines[2].end.x);

        float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
        normal.x /= l; normal.y /= l; normal.z /= l;

        if (abs( normal.x ) < EPS) normal.x = 0.0;
        if (abs( normal.y ) < EPS) normal.y = 0.0;
        if (abs( normal.z ) < EPS) normal.z = 0.0;


        std::cout << normal.x *(triRotated.lines[0].start.x - vCamera.x) +
                      normal.y *(triRotated.lines[0].start.y - vCamera.y)+
                      normal.z *(triRotated.lines[0].start.z - vCamera.z) << std::endl;
        /*if (normal.x *(triRotated.lines[0].start.x - vCamera.x) +
            normal.y *(triRotated.lines[0].start.y - vCamera.y)+
            normal.z *(triRotated.lines[0].start.z - vCamera.z) < 0.0f)*/ //doesn't work
        if (normal.z < 0.0f)
        {
            //lighting
            vec3D lightDirection = {0.0f,0.0f,-1.0f};
            float le = sqrtf(lightDirection.x*lightDirection.x + lightDirection.y*lightDirection.y + lightDirection.z*lightDirection.z);
            lightDirection.x /= le; lightDirection.y /= le; lightDirection.z /= le;
            float dp = normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;
            Uint8 clr = ceil(fabsf(dp) * 255);
            SDL_Vertex vertices[3];
            sleep(0.1);
            //projection
            for (int i = 0; i < 3; ++i) {
                vec2D startLine = projection(triRotated.lines[i].start);
                //vec2D endLine = projection(triRotated.lines[i].end);
                vertices[i].color = {0, clr, 0,255};
                vertices[i].position= {startLine.x,startLine.y};
                //SDL_RenderDrawLine(renderer, startLine.x, startLine.y, endLine.x, endLine.y);
            }
            SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);

        }




    }

    SDL_RenderPresent(renderer);
    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

float renderer3D::getDeltaTime() const {
    return DeltaTime;
}

void renderer3D::setDeltaTime(float deltaTime) {
    DeltaTime = deltaTime;
}


void renderer3D::setWindowSizeX(int windowSizeX) {
    WindowSizeX = windowSizeX;
}

void renderer3D::setWindowSizeY(int windowSizeY) {
    WindowSizeY = windowSizeY;
}



int renderer3D::getWindowSizeY() {
    return 0;
}

int renderer3D::getWindowSizeX() {
    return 0;
}


vec3D renderer3D::rotateX(vec3D point) {
    vec3D returnPoint;
    returnPoint.x = point.x;
    returnPoint.y = cos(rotation.xRotation) * point.y - sin(rotation.xRotation) * point.z;
    returnPoint.z = sin(rotation.xRotation) * point.y + cos(rotation.xRotation) * point.z;
    return returnPoint;
}

vec3D renderer3D::rotateY(vec3D point) {
    vec3D returnPoint;
    returnPoint.x = cos(rotation.yRotation) * point.x + sin(rotation.yRotation) * point.z;
    returnPoint.y = point.y;
    returnPoint.z = -sin(rotation.yRotation) * point.x + cos(rotation.yRotation) * point.z;
    return returnPoint;
}

vec3D renderer3D::rotateZ(vec3D point) {
    vec3D returnPoint;
    returnPoint.x = cos(rotation.zRotation) * point.x - sin(rotation.zRotation) * point.y;
    returnPoint.y = sin(rotation.zRotation) * point.x + cos(rotation.zRotation) * point.y;
    returnPoint.z = point.z;
    return returnPoint;
}

vec2D renderer3D::projection(vec3D point) {
    return vec2D{pos.x + (FOV * point.x) / (FOV + point.z) * scale, pos.y + (FOV * point.y) / (FOV + point.z) * scale};
}


float renderer3D::getFov() const {
    return FOV;
}

void renderer3D::setFov(float fov) {
    FOV = fov;
}

float renderer3D::getScale() const {
    return scale;
}

void renderer3D::setScale(float scale) {
    renderer3D::scale = scale;
}

const vec2D &renderer3D::getPos() const {
    return pos;
}

void renderer3D::setPos(const vec2D &pos) {
    renderer3D::pos = pos;
}

const Rotation &renderer3D::getRotation() const {
    return rotation;
}

void renderer3D::setRotation(const Rotation &rotation) {
    renderer3D::rotation = rotation;
}
