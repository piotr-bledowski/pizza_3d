#include "FlattenedSphere.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>

FlattenedSphere::FlattenedSphere(float rx, float ry, float rz, int sl, int st)
    : radiusX(rx)
    , radiusY(ry)
    , radiusZ(rz)
    , slices(sl)
    , stacks(st) {}

void FlattenedSphere::draw()
{
    TextureManager::bindCheeseTexture();
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(radiusX, radiusY, radiusZ);
    glutSolidSphere(1.0, slices, stacks);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
