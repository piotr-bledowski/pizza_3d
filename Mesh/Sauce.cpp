#include "Sauce.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
}

Sauce::Sauce(float r, float h, int s) : radius(r), height(h), segments(s) {}

void Sauce::draw() {
    TextureManager::bindSauceTexture();
    glColor3f(1.0f, 1.0f, 1.0f);

    const float halfH = height * 0.5f;
    const float step = 2.0f * PI / static_cast<float>(segments);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        const float t = static_cast<float>(i) * step;
        const float u = 0.5f + 0.5f * std::cos(t);
        const float v = 0.5f + 0.5f * std::sin(t);
        glTexCoord2f(u, v);
        glVertex3f(radius * std::cos(t), -halfH, radius * std::sin(t));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, halfH, 0.0f);
    for (int i = segments; i >= 0; --i) {
        const float t = static_cast<float>(i) * step;
        const float u = 0.5f + 0.5f * std::cos(t);
        const float v = 0.5f + 0.5f * std::sin(t);
        glTexCoord2f(u, v);
        glVertex3f(radius * std::cos(t), halfH, radius * std::sin(t));
    }
    glEnd();

    const float segf = static_cast<float>(segments);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        const float t = static_cast<float>(i) * step;
        const float x = radius * std::cos(t);
        const float z = radius * std::sin(t);
        const float u = (segf > 0.0f) ? static_cast<float>(i) / segf : 0.0f;
        glTexCoord2f(u, 0.0f);
        glVertex3f(x, -halfH, z);
        glTexCoord2f(u, 1.0f);
        glVertex3f(x, halfH, z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
