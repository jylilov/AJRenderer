#include "vertex.h"

Vertex::Vertex(const Vertex &vertex)
        : coordinates(vertex.coordinates),
          textureCoordinates(vertex.textureCoordinates),
          normalVector(vertex.normalVector)

{
}

Vertex::Vertex(const Vec3d &coordinates, const Vec2d &textureCoordinates, const Vec3d &normalVector)
        : coordinates(coordinates),
          textureCoordinates(textureCoordinates),
          normalVector(normalVector)
{
}