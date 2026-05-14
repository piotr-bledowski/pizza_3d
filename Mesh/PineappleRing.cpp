#include "PineappleRing.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
}

PineappleRing::PineappleRing(float outerR, float innerR, float thick, int ringSegs)
    : outerRadius(outerR)
    , innerRadius(innerR)
    , thickness(thick)
    , ringSegments(std::max(8, ringSegs))
{
}

void PineappleRing::draw()
{
    TextureManager::bindPineappleTexture();

    const bool baked = TextureManager::isBaked();
    if (baked)
    {
        glColor3f(kBakedColorR, kBakedColorG, kBakedColorB);
        glPushMatrix();
        glScalef(kBakedScaleXZ, kBakedScaleY, kBakedScaleXZ);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    const float halfH = thickness * 0.5f;
    const float ri = std::max(0.01f, std::min(innerRadius, outerRadius * 0.45f));
    const float ro = std::max(ri + 0.02f, outerRadius);
    const int segs = ringSegments;
    const float step = 2.0f * PI / static_cast<float>(segs);

    // Bottom annulus
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; ++i)
    {
        const float t = static_cast<float>(i) * step;
        const float c = std::cos(t);
        const float s = std::sin(t);
        const float xi = ri * c;
        const float zi = ri * s;
        const float xo = ro * c;
        const float zo = ro * s;
        glTexCoord2f(0.5f + 0.5f * (xi / ro), 0.5f + 0.5f * (zi / ro));
        glVertex3f(xi, -halfH, zi);
        glTexCoord2f(0.5f + 0.5f * (xo / ro), 0.5f + 0.5f * (zo / ro));
        glVertex3f(xo, -halfH, zo);
    }
    glEnd();

    // Top annulus
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = segs; i >= 0; --i)
    {
        const float t = static_cast<float>(i) * step;
        const float c = std::cos(t);
        const float s = std::sin(t);
        const float xi = ri * c;
        const float zi = ri * s;
        const float xo = ro * c;
        const float zo = ro * s;
        glTexCoord2f(0.5f + 0.5f * (xi / ro), 0.5f + 0.5f * (zi / ro));
        glVertex3f(xi, halfH, zi);
        glTexCoord2f(0.5f + 0.5f * (xo / ro), 0.5f + 0.5f * (zo / ro));
        glVertex3f(xo, halfH, zo);
    }
    glEnd();

    // Outer vertical wall
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segs; ++i)
    {
        const float t = static_cast<float>(i) * step;
        const float c = std::cos(t);
        const float s = std::sin(t);
        const float xo = ro * c;
        const float zo = ro * s;
        const float u = static_cast<float>(i) / static_cast<float>(segs);
        glTexCoord2f(u, 0.0f);
        glVertex3f(xo, -halfH, zo);
        glTexCoord2f(u, 1.0f);
        glVertex3f(xo, halfH, zo);
    }
    glEnd();

    // Inner vertical wall
    glBegin(GL_QUAD_STRIP);
    for (int i = segs; i >= 0; --i)
    {
        const float t = static_cast<float>(i) * step;
        const float c = std::cos(t);
        const float s = std::sin(t);
        const float xi = ri * c;
        const float zi = ri * s;
        const float u = static_cast<float>(i) / static_cast<float>(segs);
        glTexCoord2f(u, 0.0f);
        glVertex3f(xi, -halfH, zi);
        glTexCoord2f(u, 1.0f);
        glVertex3f(xi, halfH, zi);
    }
    glEnd();

    if (baked)
    {
        glPopMatrix();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
}
