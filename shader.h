#pragma once

#include "matrix.h"
#include "vertex.h"
#include "texture.h"

class AbstractShader {
public:
    virtual ~AbstractShader() {}
    virtual Vec4d vertexShader(Vertex vertex, uint nVertex) = 0;
    virtual bool fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) = 0;
};

class ShadowShader: public AbstractShader {
private:
    Mat4d uniformShadowMatrix;
public:
    ShadowShader()  {}
    virtual ~ShadowShader() {}

    void setUniformShadowMatrix(Mat4d const &uniformShadowMatrix) {
        this->uniformShadowMatrix = uniformShadowMatrix;
    }

    virtual Vec4d vertexShader(Vertex vertex, uint nVertex) {
        return uniformShadowMatrix * vertex.getCoordinates().getExtension();
    }

    virtual bool fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) {
        resultColor = VecColor();
        return false;
    }

};

class Shader: public AbstractShader {
private:
    Mat4d uniformViewport;
    Mat4d uniformProjection;
    Mat4d uniformView;
    Mat4d uniformModel;
    Mat4d uniformShadow;

    Texture *uniformDiffTexture;
    Texture *uniformNmTexture;
    Texture *uniformSpecTexture;

    Vec3d uniformLightVector;
    Buffer *uniformShadowBuffer;

    Mat4d shadowMatrix;
    Mat4d lightMatrix;
    Mat4d vertexMatrix;
    Mat4d normalMatrix;

    Matrix<3, 3, double> varyingVertexCoordinate;
    Matrix<2, 3, double> varyingTextureCoordinate;
    Matrix<3, 3, double> varyingNormalVector;

public:
    Shader() {}
    virtual ~Shader() {}

    virtual Vec4d vertexShader(Vertex vertex, uint nVertex);
    virtual bool fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor);

    void update();

    void setUniformViewport(Mat4d const &uniformViewport) {
        this->uniformViewport = uniformViewport;
    }

    void setUniformProjection(Mat4d const &uniformProjection) {
        this->uniformProjection = uniformProjection;
    }

    void setUniformView(Mat4d const &uniformView) {
        this->uniformView = uniformView;
    }

    void setUniformModel(Mat4d const &uniformModel) {
        this->uniformModel = uniformModel;
    }

    void setUniformShadow(Mat4d const &uniformShadow) {
        this->uniformShadow = uniformShadow;
    }

    void setUniformDiffTexture(Texture *uniformDiffTexture) {
        this->uniformDiffTexture = uniformDiffTexture;
    }

    void setUniformNmTexture(Texture *uniformNmTexture) {
        this->uniformNmTexture = uniformNmTexture;
    }

    void setUniformSpecTexture(Texture *uniformSpecTexture) {
        this->uniformSpecTexture = uniformSpecTexture;
    }

    void setUniformLightVector(Vec3d const &uniformLightVector) {
        this->uniformLightVector = uniformLightVector;
    }

    void setUniformShadowBuffer(Buffer *uniformShadowBuffer) {
        this->uniformShadowBuffer = uniformShadowBuffer;
    }

    double getShadowCoefficient(Vec3d &v);

    Vec3d getNormalVector(Vec2d const &vt, Vec3d const &bc);

    Vector<3, double> getLightVector();

    Vector<3, double> getReflectVector(Vec3d &n, Vec3d &l);

    qreal getSpecularCoefficient(Vec2d &vt, Vec3d &reflect);

    double const &getDiffuseCoefficient(Vec3d &n, Vec3d &l);

    double getAmbientCoefficient();

    Vector<3, double> getColor(Vec2d &vt);
};
