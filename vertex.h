#pragma once

#include "vector.h"

class Vertex {
private:
    Vec3d coordinates;
    Vec2d textureCoordinates;
    Vec3d normalVector;
public:

    Vertex() {}
    Vertex(const Vertex &vertex);
    Vertex(const Vec3d &coordinates, const Vec2d &textureCoordinates, const Vec3d &normalVector);

    Vec3d getCoordinates() const { return coordinates; }
    void setCoordinates(const Vec3d &coordinates) { Vertex::coordinates = coordinates; }
    Vec2d getTextureCoordinates() const { return textureCoordinates; }
    void setTextureCoordinates(const Vec2d &textureCoordinates) { Vertex::textureCoordinates = textureCoordinates; }
    Vec3d getNormalVector() const { return normalVector; }
    void setNormalVector(const Vec3d &normalVector) { Vertex::normalVector = normalVector; }
};
