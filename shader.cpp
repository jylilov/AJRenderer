#include "shader.h"

Vec4d Shader::vertexShader(Vertex vertex, uint nVertex) {
    Vec4d coordinates = vertex.getCoordinates().getExtension();

    coordinates = vertexMatrix * coordinates;

    varyingVertexCoordinate.setColumn(nVertex, vertex.getCoordinates());
    varyingTextureCoordinate.setColumn(nVertex, vertex.getTextureCoordinates());
    varyingNormalVector.setColumn(nVertex, vertex.getNormalVector());

    return coordinates;
}

bool Shader::fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) {

    Vec3d v = varyingVertexCoordinate * barycentricCoordinates;
    Vec2d vt = varyingTextureCoordinate * barycentricCoordinates;

    double shadow = getShadowCoefficient(v);

    Vec3d n = getNormalVector(vt, barycentricCoordinates);
    Vec3d l = getLightVector();
    Vec3d reflect = getReflectVector(n, l);

    double ambient = getAmbientCoefficient();
    double spec = getSpecularCoefficient(vt, reflect);
    double diff = getDiffuseCoefficient(n, l);

    Vec3d color = getColor(vt);

    double k = (ambient + diff + 0.4 * spec);
    if (k > 1) k = 1;

    color *= shadow * k;

    resultColor = VecColor::cast(Vec3i::cast(color));
    return false;
}

Vec3d Shader::getColor(Vec2d const &vt) {
    Vec3d color;
    if (uniformDiffTexture == 0) {
        color = Vec3d(255.0, 255.0, 255.0);
    } else {
        color = Vec3d::cast(uniformDiffTexture->get(vt));
    }
    return color;
}

double Shader::getAmbientCoefficient() {
    return 0.2;
}

double Shader::getDiffuseCoefficient(Vec3d const &n, Vec3d const &l) {
    return qMax(0.0, n * l);
}

double Shader::getSpecularCoefficient(Vec2d const &vt, Vec3d const &reflect) {
    int power;
    if (uniformSpecTexture == 0) {
        power = 1;
    } else {
        power = uniformSpecTexture->get(vt)[0];
        if (power == 0) power = 1;
    }
    return qPow(qMax(reflect[2], 0.0), power);
}

Vector<3, double> Shader::getReflectVector(Vec3d const &n, Vec3d const &l) {
    return (n * (n * l * 2.0) - l).getNormalVector();
}

Vector<3, double> Shader::getLightVector() {
    return (lightMatrix * uniformLightVector.getExtension(0.0)).getProjection().getNormalVector();
}

Vec3d Shader::getNormalVector(Vec2d const &vt, Vec3d const &bc) {
    Vec3d n;
    if (uniformNmTexture == 0) {
        n = varyingNormalVector * bc;
    } else {
        Vec3d nm = Vec3d::cast(uniformNmTexture->get(vt)) - Vec3d(128.0, 128.0, 128.0);
        n = (normalMatrix * nm.getExtension(0.0)).getProjection().getNormalVector();
    }
    return n;
}

double Shader::getShadowCoefficient(Vec3d const &v) {
    Vec3d vShadow = (shadowMatrix * v.getExtension()).getProjection();

    int x = qRound(vShadow[0]);
    int y = qRound(vShadow[1]);

    double shadow;
    if (uniformShadowBuffer->get(x, y)[0] > static_cast<uchar>(vShadow[2]) + 20) {
        shadow = 0.6;
    } else {
        shadow = 1;
    }
    return shadow;
}

void Shader::update()
{
    Mat4d vertexModelMatrix = uniformProjection * uniformView * uniformModel;
    vertexMatrix = uniformViewport * vertexModelMatrix;
    normalMatrix = vertexModelMatrix.getInverseMatrix().getTransposeMatrix();
    shadowMatrix = uniformShadow * uniformModel;
    lightMatrix = uniformView;
}
