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

    //projection
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)WindowSizeY / (float)WindowSizeX;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

}


//=================================================================================

void renderer3D::render() {
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);


    mat4x4 matRotZ, matRotX, matRotY;
    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(rotation.xRotation);
    matRotX.m[1][2] = sinf(rotation.xRotation);
    matRotX.m[2][1] = -sinf(rotation.xRotation);
    matRotX.m[2][2] = cosf(rotation.xRotation);
    matRotX.m[3][3] = 1;
    // Rotation Z
    matRotZ.m[0][0] = cosf(rotation.zRotation);
    matRotZ.m[0][1] = sinf(rotation.zRotation);
    matRotZ.m[1][0] = -sinf(rotation.zRotation);
    matRotZ.m[1][1] = cosf(rotation.zRotation);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;
    // Rotation Y
    matRotY.m[0][0] = cosf(rotation.yRotation);
    matRotY.m[2][0] = sinf(rotation.yRotation);
    matRotY.m[0][2] = -sinf(rotation.yRotation);
    matRotY.m[2][2] = cosf(rotation.yRotation);
    matRotY.m[1][1] = 1;
    matRotY.m[3][3] = 1;


    //Auto Rotation
    rotation.xRotation += 1* DeltaTime;
    rotation.yRotation += 1*DeltaTime;
    rotation.zRotation += 1*DeltaTime;

    SDL_SetRenderDrawColor(renderer3D::renderer,255,255,255,SDL_ALPHA_OPAQUE);
    for (auto& poly : meshObject.polygons) {
        polygon triTranslated, triRotatedx, triRotatedxy, triRotated, triProjected;

        //rotate points
        for (int i = 0; i < 3; ++i) {
            MultiplyMatrixVector(poly.points[i],triRotatedx.points[i],matRotX);
        }
        for (int i = 0; i < 3; ++i) {
            MultiplyMatrixVector(triRotatedx.points[i],triRotatedxy.points[i],matRotY);
        }
        for (int i = 0; i < 3; ++i) {
            MultiplyMatrixVector(triRotatedxy.points[i],triRotated.points[i],matRotZ);
        }


        // Offset into the screen
        triTranslated = triRotated;
        triTranslated.points[0].z = triRotated.points[0].z + 3.0f;
        triTranslated.points[1].z = triRotated.points[1].z + 3.0f;
        triTranslated.points[2].z = triRotated.points[2].z + 3.0f;


        // Use Cross-Product to get surface normal
        vec3D normal, edge1, edge2;
        edge1.x = triTranslated.points[1].x - triTranslated.points[0].x;
        edge1.y = triTranslated.points[1].y - triTranslated.points[0].y;
        edge1.z = triTranslated.points[1].z - triTranslated.points[0].z;

        edge2.x = triTranslated.points[2].x - triTranslated.points[0].x;
        edge2.y = triTranslated.points[2].y - triTranslated.points[0].y;
        edge2.z = triTranslated.points[2].z - triTranslated.points[0].z;

        //croos product
        normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
        normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
        normal.z = edge1.x * edge2.y - edge1.y * edge2.x;

        if (normal.x == 0.0f) normal.x = 0.0f;
        if (normal.y == 0.0f) normal.y = 0.0f;
        if (normal.z == 0.0f) normal.z = 0.0f;

        float l = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
        normal.x /= l; normal.y /= l; normal.z /= l;

        //dot product
        if (normal.x *(triTranslated.points[0].x - vCamera.x) +
            normal.y *(triTranslated.points[0].y - vCamera.y)+
            normal.z *(triTranslated.points[0].z - vCamera.z) < 0.0f) //doesn't work
        //if (normal.z < 0.0f)
        {
            //lighting
            vec3D lightDirection = {0.0f,0.0f,-1.0f};
            float le = sqrt(lightDirection.x*lightDirection.x + lightDirection.y*lightDirection.y + lightDirection.z*lightDirection.z);
            lightDirection.x /= le; lightDirection.y /= le; lightDirection.z /= le;
            float dp = normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;
            Uint8 clr = ceil(fabsf((dp+1)/2) * 255);


            //projection
            MultiplyMatrixVector(triTranslated.points[0], triProjected.points[0], matProj);
            MultiplyMatrixVector(triTranslated.points[1], triProjected.points[1], matProj);
            MultiplyMatrixVector(triTranslated.points[2], triProjected.points[2], matProj);

            triProjected.points[0].x += 1.0f; triProjected.points[0].y += 1.0f;
            triProjected.points[1].x += 1.0f; triProjected.points[1].y += 1.0f;
            triProjected.points[2].x += 1.0f; triProjected.points[2].y += 1.0f;
            triProjected.points[0].x *= 0.5f * (float)WindowSizeX;
            triProjected.points[0].y *= 0.5f * (float)WindowSizeY;
            triProjected.points[1].x *= 0.5f * (float)WindowSizeX;
            triProjected.points[1].y *= 0.5f * (float)WindowSizeY;
            triProjected.points[2].x *= 0.5f * (float)WindowSizeX;
            triProjected.points[2].y *= 0.5f * (float)WindowSizeY;

            SDL_Vertex vertices[3]; //triangle points quordinates


            for (int i = 0; i < 3; ++i) {
                vertices[i].color = {0, clr, clr,255};
                vertices[i].position= {triProjected.points[i].x,triProjected.points[i].y};
            }

            //draw triangle
            SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);

            //draw outlines
            //SDL_RenderDrawLine(renderer, triProjected.points[0].x, triProjected.points[0].y, triProjected.points[1].x, triProjected.points[1].y);
            //SDL_RenderDrawLine(renderer, triProjected.points[1].x, triProjected.points[1].y, triProjected.points[2].x, triProjected.points[2].y);
            //SDL_RenderDrawLine(renderer, triProjected.points[2].x, triProjected.points[2].y, triProjected.points[0].x, triProjected.points[0].y);



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


void renderer3D::rotateX(vec3D &point, vec3D &pointr) {
    pointr.x = point.x;
    pointr.y = cos(rotation.xRotation) * point.y - sin(rotation.xRotation) * point.z;
    pointr.z = sin(rotation.xRotation) * point.y + cos(rotation.xRotation) * point.z;
}

void renderer3D::rotateY(vec3D &point, vec3D &pointr) {

    pointr.y = point.y;
    pointr.x = cos(rotation.yRotation) * point.x + sin(rotation.yRotation) * point.z;
    pointr.z = -sin(rotation.yRotation) * point.x + cos(rotation.yRotation) * point.z;
}

void renderer3D::rotateZ(vec3D &point, vec3D &pointr) {
    pointr.x = cos(rotation.zRotation) * point.x - sin(rotation.zRotation) * point.y;
    pointr.y = sin(rotation.zRotation) * point.x + cos(rotation.zRotation) * point.y;
    pointr.z = point.z;
}

void renderer3D::projection(vec3D &point, vec2D &pointr) {
    pointr = vec2D{pos.x + (FOV * point.x) / (FOV + point.z) * scale, pos.y + (FOV * point.y) / (FOV + point.z) * scale};
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