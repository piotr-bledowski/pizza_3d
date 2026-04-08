#include "CheeseCuboid.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>

CheeseCuboid::CheeseCuboid(float w, float h, float d) : Cuboid(w, h, d) {}

void CheeseCuboid::draw() {
    glDisable(GL_TEXTURE_2D);
    if (TextureManager::isBaked()) {
        glColor3f(0.84f, 0.73f, 0.34f);
    } else {
        glColor3f(0.95f, 0.9f, 0.45f);
    }
    Cuboid::draw();
    glColor3f(1.0f, 1.0f, 1.0f);
}
