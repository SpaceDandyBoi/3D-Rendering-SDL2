//
// Created by Dandy on 7/26/2023.
//

#include "renderer3d.h"

#include <utility>

renderer3D::renderer3D(mesh P_meshObject, SDL_Window* window, SDL_Renderer* p_renderer){
    SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);
    meshObject = std::move(P_meshObject);
    renderer = p_renderer;
    pos = Point2D(WindowSizeX/2,WindowSizeY/2);
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
    for (auto& verte : meshObject.pmesh) {

        int j = 0;
        //lines
        Point3D rotatedStartPoint0 = rotateX(rotateY(verte.poly[0].start));
        Point3D rotatedEndPoint0 = rotateX(rotateY(verte.poly[0].end));
        Point3D rotatedStartPoint1 = rotateX(rotateY(verte.poly[1].start));
        Point3D rotatedEndPoint1 = rotateX(rotateY(verte.poly[1].end));
        Point3D rotatedStartPoint2 = rotateX(rotateY(verte.poly[2].start));
        Point3D rotatedEndPoint2 = rotateX(rotateY(verte.poly[2].end));

        //normals




        //projection
        Point2D start0 = projection(rotatedStartPoint0);
        Point2D end0 = projection(rotatedEndPoint0);
        Point2D start1 = projection(rotatedStartPoint1);
        Point2D end1 = projection(rotatedEndPoint1);
        Point2D start2 = projection(rotatedStartPoint2);
        Point2D end2 = projection(rotatedEndPoint2);

        SDL_RenderDrawLine(renderer, start0.x, start0.y, end0.x, end0.y);
        SDL_RenderDrawLine(renderer, start1.x, start1.y, end1.x, end1.y);
        SDL_RenderDrawLine(renderer, start2.x, start2.y, end2.x, end2.y);


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


Point3D renderer3D::rotateX(Point3D point) {
    Point3D returnPoint;
    returnPoint.x = point.x;
    returnPoint.y = cos(rotation.xRotation) * point.y - sin(rotation.xRotation) * point.z;
    returnPoint.z = sin(rotation.xRotation) * point.y + cos(rotation.xRotation) * point.z;
    return returnPoint;
}

Point3D renderer3D::rotateY(Point3D point) {
    Point3D returnPoint;
    returnPoint.x = cos(rotation.yRotation) * point.x - sin(rotation.yRotation) * point.z;
    returnPoint.y = point.y;
    returnPoint.z = sin(rotation.yRotation) * point.x + cos(rotation.yRotation) * point.z;
    return returnPoint;
}

Point2D renderer3D::projection(Point3D point) {
    return Point2D{pos.x + (FOV * point.x) / (FOV + point.z) * scale, pos.y + (FOV * point.y) / (FOV + point.z) * scale};
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

const Point2D &renderer3D::getPos() const {
    return pos;
}

void renderer3D::setPos(const Point2D &pos) {
    renderer3D::pos = pos;
}

const Rotation &renderer3D::getRotation() const {
    return rotation;
}

void renderer3D::setRotation(const Rotation &rotation) {
    renderer3D::rotation = rotation;
}
