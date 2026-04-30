#include "Pea.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>

Pea::Pea(float r, int sl, int st) : Sphere(r, sl, st) {}

void Pea::draw() {
    if (TextureManager::isBaked()) {
        glColor3f(0.14f, 0.48f, 0.22f);
    } else {
        glColor3f(0.18f, 0.62f, 0.28f);
    }
    Sphere::draw();
    glColor3f(1.0f, 1.0f, 1.0f);
}
