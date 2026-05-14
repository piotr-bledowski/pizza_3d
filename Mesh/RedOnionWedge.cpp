#include "RedOnionWedge.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>

namespace
{
    constexpr float PI = 3.14159265358979323846f;
}

RedOnionWedge::RedOnionWedge(float outerR, float innerR, float wedgeRad, float thick, int arcSegs)
    : outerRadius(outerR), innerRadius(innerR), wedgeAngle(wedgeRad), thickness(thick), arcSegments(std::max(6, arcSegs))
{
}

void RedOnionWedge::draw()
{
    TextureManager::bindRedOnionTexture();

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

    // Flat curved ribbon.  The texture V axis runs from 0 (inner = white) to
    // 1 (outer = purple), so each face explicitly pins V to the right end.
    const float halfH = thickness * 0.5f;
    const float ri = std::max(0.004f, innerRadius);
    const float ro = std::max(ri + 0.004f, outerRadius);
    const float halfW = wedgeAngle * 0.5f;
    const int segs = arcSegments;
    const float step = wedgeAngle / static_cast<float>(segs);

    // ── Top face (y = +halfH): V=0 at inner (white), V=1 at outer (purple) ──
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; ++i)
    {
        const float phi = -halfW + static_cast<float>(i) * step;
        const float sp = std::sin(phi), cp = std::cos(phi);
        const float u = static_cast<float>(i) / static_cast<float>(segs);
        glTexCoord2f(u, 0.0f);
        glVertex3f(ri * sp, +halfH, ri * cp);
        glTexCoord2f(u, 1.0f);
        glVertex3f(ro * sp, +halfH, ro * cp);
    }
    glEnd();

    // ── Bottom face (y = -halfH) – reversed winding ─────────────────────────
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = segs; i >= 0; --i)
    {
        const float phi = -halfW + static_cast<float>(i) * step;
        const float sp = std::sin(phi), cp = std::cos(phi);
        const float u = static_cast<float>(i) / static_cast<float>(segs);
        glTexCoord2f(u, 0.0f);
        glVertex3f(ri * sp, -halfH, ri * cp);
        glTexCoord2f(u, 1.0f);
        glVertex3f(ro * sp, -halfH, ro * cp);
    }
    glEnd();

    // ── Outer curved wall – V=1 throughout (purple) ─────────────────────────
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segs; ++i)
    {
        const float phi = -halfW + static_cast<float>(i) * step;
        const float sp = std::sin(phi), cp = std::cos(phi);
        const float u = static_cast<float>(i) / static_cast<float>(segs);
        glTexCoord2f(u, 1.0f);
        glVertex3f(ro * sp, -halfH, ro * cp);
        glTexCoord2f(u, 1.0f);
        glVertex3f(ro * sp, +halfH, ro * cp);
    }
    glEnd();

    // ── Inner curved wall – V=0 throughout (white), reversed winding ─────────
    glBegin(GL_QUAD_STRIP);
    for (int i = segs; i >= 0; --i)
    {
        const float phi = -halfW + static_cast<float>(i) * step;
        const float sp = std::sin(phi), cp = std::cos(phi);
        const float u = static_cast<float>(i) / static_cast<float>(segs);
        glTexCoord2f(u, 0.0f);
        glVertex3f(ri * sp, -halfH, ri * cp);
        glTexCoord2f(u, 0.0f);
        glVertex3f(ri * sp, +halfH, ri * cp);
    }
    glEnd();

    // ── End caps: inner edge V=0 (white), outer edge V=1 (purple) ────────────
    for (int side = 0; side < 2; ++side)
    {
        const float phi = (side == 0) ? -halfW : halfW;
        const float sp = std::sin(phi), cp = std::cos(phi);
        glBegin(GL_QUADS);
        if (side == 0)
        {
            glTexCoord2f(0.5f, 1.0f);
            glVertex3f(ro * sp, -halfH, ro * cp);
            glTexCoord2f(0.5f, 1.0f);
            glVertex3f(ro * sp, +halfH, ro * cp);
            glTexCoord2f(0.5f, 0.0f);
            glVertex3f(ri * sp, +halfH, ri * cp);
            glTexCoord2f(0.5f, 0.0f);
            glVertex3f(ri * sp, -halfH, ri * cp);
        }
        else
        {
            glTexCoord2f(0.5f, 0.0f);
            glVertex3f(ri * sp, -halfH, ri * cp);
            glTexCoord2f(0.5f, 0.0f);
            glVertex3f(ri * sp, +halfH, ri * cp);
            glTexCoord2f(0.5f, 1.0f);
            glVertex3f(ro * sp, +halfH, ro * cp);
            glTexCoord2f(0.5f, 1.0f);
            glVertex3f(ro * sp, -halfH, ro * cp);
        }
        glEnd();
    }

    if (baked)
    {
        glPopMatrix();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
}
