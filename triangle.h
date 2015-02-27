#include "vector.h"

class Triangle {
private:
    Vec4d vertex1, vertex2, vertex3;
    Vec3d textureVertex1, textureVertex2, textureVertex3;
public:
    Triangle() {}

    Vec4d getVertex1() const { return vertex1; }
    void setVertex1(Vec4d vertex1) { Triangle::vertex1 = vertex1; }

    Vec4d getVertex2() const { return vertex2; }
    void setVertex2(Vec4d vertex2) { Triangle::vertex2 = vertex2; }

    Vec4d getVertex3() const { return vertex3; }
    void setVertex3(Vec4d vertex3) { Triangle::vertex3 = vertex3; }

    Vec4d getVertex(size_t index) const;
    void setVertex(size_t index, Vec4d vertex);

    Vec3d getTextureVertex1() const { return textureVertex1; }
    void setTextureVertex1(Vec3d textureVertex1) { Triangle::textureVertex1 = textureVertex1; }

    Vec3d getTextureVertex2() const { return textureVertex2; }
    void setTextureVertex2(Vec3d textureVertex2) { Triangle::textureVertex2 = textureVertex2; }

    Vec3d getTextureVertex3() const { return textureVertex3; }
    void setTextureVertex3(Vec3d textureVertex3) { Triangle::textureVertex3 = textureVertex3; }

    Vec3d getTextureVertex(size_t index) const;
    void setTextureVertex(size_t index, Vec3d vertex);
};