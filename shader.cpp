#include "shader.h"

Vec4d Shader::vertexShader(Vertex vertex, uint nVertex) {
    Vec4d coordinates = vertex.getCoordinates().getExtension();

    coordinates = vertexMatrix * coordinates;

    varyingVertexCoordinate.setColumn(nVertex, vertex.getCoordinates());
    varyingTextureCoordinate.setColumn(nVertex, vertex.getTextureCoordinates());

    return coordinates;
}

bool Shader::fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) {

    Vec3d v = varyingVertexCoordinate * barycentricCoordinates;
    Vec2d vt = varyingTextureCoordinate * barycentricCoordinates;

    double shadow = getShadowCoefficient(v);

    Vec3d n = getNormalVector(vt);
    Vec3d l = getLightVector();
    Vec3d reflect = getReflectVector(n, l);

    double ambient = getAmbientCoefficient();
    double spec = getSpecularCoefficient(vt, reflect);
    double diff = getDiffuseCoefficient(n, l);

    Vec3d color = Vec3d::cast(uniformDiffTexture->get(vt));
//    Vec3d color = Vec3d(255.0, 255.0, 255.0);

    double k = (ambient + diff + 0.4 * spec);
//    double k = spec;
//    double k = diff;
//    double k = 1;

    color *= shadow * k;
//    color *= k;

    resultColor = VecColor::cast(Vec3i::cast(color));
    return false;
}

double Shader::getAmbientCoefficient() {
    return 0.2;
}

double const &Shader::getDiffuseCoefficient(Vec3d &n, Vec3d &l) {
    return qMax(0.0, n * l);
}

qreal Shader::getSpecularCoefficient(Vec2d &vt, Vec3d &reflect) {
    return qPow(qMax(reflect[2], 0.0), uniformSpecTexture->get(vt)[0]);
}

Vector<3, double> Shader::getReflectVector(Vec3d &n, Vec3d &l) {
    return (n * (n * l * 2.0) - l).getNormalVector();
}

Vector<3, double> Shader::getLightVector() {
    return (lightMatrix * uniformLightVector.getExtension(0.0)).getProjection().getNormalVector();
}

Vec3d Shader::getNormalVector(Vec2d &vt) {
    Vec3d nm = Vec3d::cast(uniformNmTexture->get(vt)) - Vec3d(128.0, 128.0, 128.0);
    Vec3d n = (normalMatrix * nm.getExtension(0.0)).getProjection().getNormalVector();
    return n;
}

double Shader::getShadowCoefficient(Vec3d &v) {
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
