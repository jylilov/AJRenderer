#include "shader.h"

Vec4d Shader::vertexShader(Vertex vertex) {
    Vec4d coordinates = vertex.getCoordinates().getExtension();

    coordinates = vertexMatrix * coordinates;
    verticesCoordinates.append(vertex.getCoordinates());

    textureCoordinates.append(vertex.getTextureCoordinates());

    return coordinates;
}

bool Shader::fragmentShader(Vec3d barycentricCoordinates, VecColor &resultColor) {
    interpolate(barycentricCoordinates);

    Vec3d sb_p = (shadow * vertexCoordinate.getExtension()).getProjection();

    int x = qRound(sb_p[0]);
    int y = qRound(sb_p[1]);

    double shadow;
    if (shadowBuffer->get(x, y)[0] > static_cast<uchar>(sb_p[2]) + 20) {
        shadow = 0.6;
    } else {
        shadow = 1;
    }

    Vec3d nm = Vec3d::cast(nmTexture->get(textureCoordinate)) / 255.0 * 2.0 + Vec3d(-1.0, -1.0, -1.0);

    Vec3d n = (normalMatrix * nm.getExtension(0.0)).getProjection().getNormalVector();
    Vec3d l = (view * lightVector.getExtension(0.0)).getProjection().getNormalVector();

    Vec3d reflect = (n * (n * l * 2.0) - l).getNormalVector();

    double spec = qPow(qMax(reflect[2], 0.0), specTexture->get(textureCoordinate)[0]);
    double diff = qMax(0.0, n * l);

    Vec3d color = Vec3d::cast(diffTexture->get(textureCoordinate));
//    qDebug() << textureCoordinate[0] << textureCoordinate[1];
//    qDebug() << color[0] << color[1] << color[2];
//    Vec3d color = Vec3d(255.0, 255.0, 255.0);

    double k = (0.2 + diff + 0.4 * spec);
//    double k = spec;
//    double k = diff;
//    double k = 1;

    color *= shadow * k;
//    color *= k;

    resultColor = VecColor::cast(Vec3i::cast(color));
    return false;
}

void Shader::interpolate(Vec3d barycentricCoordinates) {
    Matrix<3, 2, double> textureCoordinateMatrix;
    Matrix<3, 3, double> vertexCoordinateMatrix;

    for (uint i = 0; i < 3; ++i) {
        textureCoordinateMatrix[i] = textureCoordinates.at(currentVertex + i);
        vertexCoordinateMatrix[i] = verticesCoordinates.at(currentVertex + i);
    }

    textureCoordinate = textureCoordinateMatrix.getTransposeMatrix() * barycentricCoordinates;
    vertexCoordinate = vertexCoordinateMatrix.getTransposeMatrix() * barycentricCoordinates;
}

bool Shader::nextTriangle() {
    if (verticesCoordinates.size() - currentVertex < 3) {
        return false;
    } else {
        currentVertex += 3;
        return true;
    }
}

Shader::Shader(Mat4d viewport,
        Mat4d projection,
        Mat4d view,
        Mat4d model,
        Mat4d shadow,
        Buffer *shadowBuffer,
        Vec3d lightVector,
        Texture *diffTexture,
        Texture *nmTexture,
        Texture *specTexture)
    :
        viewport(viewport),
        projection(projection),
        view(view),
        model(model),
        shadow(shadow * model),
        shadowBuffer(shadowBuffer),
        lightVector(lightVector),
        diffTexture(diffTexture),
        nmTexture(nmTexture),
        specTexture(specTexture),
        vertexMatrix(viewport * projection * view * model),
        normalMatrix((projection * view * model).getTransposeMatrix().getInverseMatrix()),
        currentVertex(0)
{
}
