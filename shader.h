#pragma once

#include "matrix.h"
#include "vertex.h"
#include "texture.h"

class IShader {
public:
    virtual ~IShader() {}
    virtual Vec4d vertexShader(Vertex vertex) = 0;
    virtual bool fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) = 0;
    virtual bool nextTriangle() = 0;
};

class ShadowShader: public IShader {
private:
    Mat4d matrix;

    ShadowShader() {}
public:
    ShadowShader(Mat4d const &matrix) : matrix(matrix) {}
    virtual ~ShadowShader() {}

    virtual Vec4d vertexShader(Vertex vertex) {
        return matrix * vertex.getCoordinates().getExtention();
    }

    virtual bool fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) {
        resultColor = VecColor();
        return false;
    }

    virtual bool nextTriangle() { return true; };
};

class Shader: public IShader {
private:
    Mat4d viewport;
    Mat4d projection;
    Mat4d view;
    Mat4d model;
    Mat4d shadow;

    Texture *diffTexture;
    Texture *nmTexture;
    Texture *specTexture;

    Vec3d lightVector;
    Buffer *shadowBuffer;

    Mat4d vertexMatrix;
    Mat4d normalMatrix;

    QList<Vec3d> verticesCoordinates;
    QList<Vec2d> textureCoordinates;
    uint currentVertex;

    Vec2d textureCoordinate;
    Vec3d vertexCoourdinate;

    Shader() {}
public:
    Shader(Mat4d viewport,
            Mat4d projection,
            Mat4d view,
            Mat4d model,
            Mat4d shadow,
            Buffer *shadowBuffer,
            Vec3d lightVector,
            Texture *diffTexture,
            Texture *nmTexture,
            Texture *specTexture
    );
    virtual ~Shader() {}

    virtual Vec4d vertexShader(Vertex vertex);
    virtual bool fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor);

    virtual bool nextTriangle();
private:
    void interpolate(Vec3d barycentricCoordinates);
};