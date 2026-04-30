#include "CuppedPepperoni.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
}

CuppedPepperoni::CuppedPepperoni(float r, float t, int s)
    : radius(r)
    , thickness(t)
    , segments(s) {}

void CuppedPepperoni::draw()
{
    TextureManager::bindPepperoniTexture();
    glColor3f(1.0f, 1.0f, 1.0f);

    const float baseY = -0.5f * thickness;
    const float rimLift = std::max(0.01f, thickness * 1.2f);
    const float step = 2.0f * PI / static_cast<float>(segments);
    const int rings = 7;

    // Concave top surface: center low, rim high ("cupped" pepperoni).
    for (int r = 0; r < rings; ++r) {
        const float t0 = static_cast<float>(r) / static_cast<float>(rings);
        const float t1 = static_cast<float>(r + 1) / static_cast<float>(rings);
        const float rr0 = radius * t0;
        const float rr1 = radius * t1;
        const float y0 = baseY + rimLift * (t0 * t0);
        const float y1 = baseY + rimLift * (t1 * t1);

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; ++i) {
            const float a = static_cast<float>(i) * step;
            const float ca = std::cos(a);
            const float sa = std::sin(a);

            glTexCoord2f(0.5f + 0.5f * t0 * ca, 0.5f + 0.5f * t0 * sa);
            glVertex3f(rr0 * ca, y0, rr0 * sa);

            glTexCoord2f(0.5f + 0.5f * t1 * ca, 0.5f + 0.5f * t1 * sa);
            glVertex3f(rr1 * ca, y1, rr1 * sa);
        }
        glEnd();
    }

    // Underside stays flatter so it still sits on the pizza.
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, -0.5f * thickness, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        const float a = static_cast<float>(i) * step;
        glTexCoord2f(0.5f + 0.5f * std::cos(a), 0.5f + 0.5f * std::sin(a));
        glVertex3f(radius * std::cos(a), -0.5f * thickness, radius * std::sin(a));
    }
    glEnd();

    // Rim wall from underside to raised cup rim.
    const float rimY = baseY + rimLift;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        const float a = static_cast<float>(i) * step;
        const float x = radius * std::cos(a);
        const float z = radius * std::sin(a);
        const float u = static_cast<float>(i) / static_cast<float>(segments);
        glTexCoord2f(u, 0.0f);
        glVertex3f(x, -0.5f * thickness, z);
        glTexCoord2f(u, 1.0f);
        glVertex3f(x, rimY, z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
