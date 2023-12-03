//
// Created by Dandy on 7/26/2023.
//

#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <strstream>
#include <algorithm>


struct vec2D {float x, y; };

struct vec3D{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1.0f;
    };

struct polygon{
    vec3D points[3];
    SDL_Color color;};

struct mesh{
    std::vector<polygon> polygons;

    bool LoadFromObjectFile(std::string sFilename)
    {
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        // Local cache of verts
        std::vector<vec3D> verts;

        while (!f.eof())
        {
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                vec3D v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if (line[0] == 'f')
            {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                polygons.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
            }
        }

        return true;
    }
};

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

    const vec3D &getVCamera() const;

    void setVCamera(const vec3D &vCamera);

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
    float scale = 1000.0f;
    int WindowSizeX;
    int WindowSizeY;
    float DeltaTime = 0.0f;

    SDL_Renderer* renderer;
    vec2D pos;

    mesh meshObject;
    vec3D vCamera ={0,0,0};
    vec3D vLookDir;
    mat4x4 matProj;

    vec3D Matrix_MultiplyVector(mat4x4 &m, vec3D &i)
    {
        vec3D v;
        v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
        v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
        v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
        v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
        return v;

    }

    mat4x4 Matrix_MakeIdentity()
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationX(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[1][2] = sinf(fAngleRad);
        matrix.m[2][1] = -sinf(fAngleRad);
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationY(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][2] = sinf(fAngleRad);
        matrix.m[2][0] = -sinf(fAngleRad);
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationZ(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][1] = sinf(fAngleRad);
        matrix.m[1][0] = -sinf(fAngleRad);
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeTranslation(float x, float y, float z)
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        matrix.m[3][0] = x;
        matrix.m[3][1] = y;
        matrix.m[3][2] = z;
        return matrix;
    }

    mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
    {
        std::cout << fAspectRatio << std::endl;
        float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
        std::cout << fFovRad << std::endl;
        mat4x4 matrix;
        matrix.m[0][0] = fAspectRatio * fFovRad;
        matrix.m[1][1] = fFovRad;
        matrix.m[2][2] = fFar / (fFar - fNear);
        matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matrix.m[2][3] = 1.0f;
        matrix.m[3][3] = 0.0f;
        return matrix;
    }

    mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2)
    {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
        return matrix;
    }
    vec3D Vector_Add(vec3D &v1, vec3D &v2)
    {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    vec3D Vector_Sub(vec3D &v1, vec3D &v2)
    {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    vec3D Vector_Mul(vec3D &v1, float k)
    {
        return { v1.x * k, v1.y * k, v1.z * k };
    }

    vec3D Vector_Div(vec3D &v1, float k)
    {
        return { v1.x / k, v1.y / k, v1.z / k };
    }

    float Vector_DotProduct(vec3D &v1, vec3D &v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
    }

    float Vector_Length(vec3D &v)
    {
        return sqrtf(Vector_DotProduct(v, v));
    }

    vec3D Vector_Normalise(vec3D &v)
    {
        float l = Vector_Length(v);
        return { v.x / l, v.y / l, v.z / l };
    }

    vec3D Vector_CrossProduct(vec3D &v1, vec3D &v2)
    {
        vec3D v;
        v.x = v1.y * v2.z - v1.z * v2.y;
        v.y = v1.z * v2.x - v1.x * v2.z;
        v.z = v1.x * v2.y - v1.y * v2.x;
        return v;
    }

    mat4x4 Matrix_PointAt(vec3D &pos, vec3D &target, vec3D &up)
    {
        // Calculate new forward direction
        vec3D newForward = Vector_Sub(target, pos);
        newForward = Vector_Normalise(newForward);

        // Calculate new Up direction
        vec3D a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
        vec3D newUp = Vector_Sub(up, a);
        newUp = Vector_Normalise(newUp);

        // New Right direction is easy, its just cross product
        vec3D newRight = Vector_CrossProduct(newUp, newForward);

        // Construct Dimensioning and Translation Matrix
        mat4x4 matrix;
        matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
        return matrix;

    }

    mat4x4 Matrix_QuickInverse(mat4x4 &m) // Only for Rotation/Translation Matrices
    {
        mat4x4 matrix;
        matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
        matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
        matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }


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
