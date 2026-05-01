#include "Cylinder.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include "../Texture/TextureManager.h"

constexpr float PI = 3.14159265358979323846f;

float g_edgeRadius = 0.15f;
int g_edgeSegments = 16;

Cylinder::Cylinder(float r, float h, int s, int slices)
    : radius(r), height(h), segments(s), sliceCount(slices)
{
}

void Cylinder::draw()
{
    TextureManager::bindPizzaTexture();

    const float halfH = height * 0.5f;
    const float edgeR = std::min(g_edgeRadius, halfH);

    // crustTubeR grows with puff; tube bottom is pinned to -halfH so it
    // rises visibly above the flat pizza surface when puffed.
    const float crustTubeR = edgeR * (1.0f + crustPuff * 0.7f);
    const float tubeCenterY = crustTubeR - halfH;
    const float innerR = radius - crustTubeR;

    // Top of crust tube — used to cap the slice-cut inner wall cleanly.
    const float crustTopY = tubeCenterY + crustTubeR;

    const int slices = sliceCount > 1 ? sliceCount : 1;
    const float sliceAngle = 2.0f * PI / static_cast<float>(slices);
    const float gap = slices > 1 ? 0.012f : 0.0f;
    const int segPerSlice = std::max(2, segments / slices);

    for (int slice = 0; slice < slices; ++slice)
    {
        const bool isHovered = (slices > 1 && hoveredSlice == slice);
        const float shade = isHovered ? (1.0f + hoverLightBoost) : 1.0f;
        glColor3f(shade, shade, shade);

        const float start = slice * sliceAngle + gap;
        const float end = (slice + 1) * sliceAngle - gap;
        const float step = (end - start) / static_cast<float>(segPerSlice);

        // ── Flat inner top face ──────────────────────────────────────────
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, halfH, 0.0f);
        for (int i = 0; i <= segPerSlice; ++i)
        {
            const float theta = start + i * step;
            const float u = 0.5f + 0.5f * cos(theta);
            const float v = 0.5f + 0.5f * sin(theta);
            glTexCoord2f(u, v);
            glVertex3f(cos(theta) * innerR, halfH, sin(theta) * innerR);
        }
        glEnd();

        // ── Flat inner bottom face ───────────────────────────────────────
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -halfH, 0.0f);
        for (int i = segPerSlice; i >= 0; --i)
        {
            const float theta = start + i * step;
            const float u = 0.5f + 0.5f * cos(theta);
            const float v = 0.5f + 0.5f * sin(theta);
            glTexCoord2f(u, v);
            glVertex3f(cos(theta) * innerR, -halfH, sin(theta) * innerR);
        }
        glEnd();

        // ── Rounded crust ring ───────────────────────────────────────────
        // Each band spans phi0..phi1 around the torus tube.
        for (int j = 0; j < g_edgeSegments; ++j)
        {
            const float phi0 = (j / static_cast<float>(g_edgeSegments)) * PI - PI * 0.5f;
            const float phi1 = ((j + 1) / static_cast<float>(g_edgeSegments)) * PI - PI * 0.5f;

            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i <= segPerSlice; ++i)
            {
                const float theta = start + i * step;
                const float cx = cos(theta) * innerR;
                const float cz = sin(theta) * innerR;

                const float x0 = cx + cos(theta) * cos(phi0) * crustTubeR;
                const float y0 = tubeCenterY + sin(phi0) * crustTubeR;
                const float z0 = cz + sin(theta) * cos(phi0) * crustTubeR;

                const float x1 = cx + cos(theta) * cos(phi1) * crustTubeR;
                const float y1 = tubeCenterY + sin(phi1) * crustTubeR;
                const float z1 = cz + sin(theta) * cos(phi1) * crustTubeR;

                glTexCoord2f(static_cast<float>(i) / segPerSlice,
                             0.3f + 0.7f * (j / static_cast<float>(g_edgeSegments)));
                glVertex3f(x0, y0, z0);
                glTexCoord2f(static_cast<float>(i) / segPerSlice,
                             0.3f + 0.7f * ((j + 1) / static_cast<float>(g_edgeSegments)));
                glVertex3f(x1, y1, z1);
            }
            glEnd();
        }

        // Close the inner side of the crust ring with a rounded profile.
        if (crustTopY > halfH)
        {
            const float innerRise = crustTopY - halfH;
            const int innerBands = 6;
            for (int band = 0; band < innerBands; ++band)
            {
                const float t0 = band / static_cast<float>(innerBands);
                const float t1 = (band + 1) / static_cast<float>(innerBands);

                const float y0 = halfH + innerRise * t0;
                const float y1 = halfH + innerRise * t1;
                // Rounded bulge in the middle; matches endpoints exactly.
                const float r0 = innerR - innerRise * 0.55f * sin(PI * t0);
                const float r1 = innerR - innerRise * 0.55f * sin(PI * t1);

                glBegin(GL_QUAD_STRIP);
                for (int i = 0; i <= segPerSlice; ++i)
                {
                    const float theta = start + i * step;
                    const float ux = cos(theta);
                    const float uz = sin(theta);
                    const float u = static_cast<float>(i) / segPerSlice;

                    glTexCoord2f(u, t0);
                    glVertex3f(ux * r0, y0, uz * r0);
                    glTexCoord2f(u, t1);
                    glVertex3f(ux * r1, y1, uz * r1);
                }
                glEnd();
            }
        }

        // ── Radial slice-cut caps ────────────────────────────────────────
        if (slices > 1)
        {
            for (int side = 0; side < 2; ++side)
            {
                const float theta = (side == 0) ? start : end;
                const float ux = cos(theta);
                const float uz = sin(theta);
                const float ex = ux * innerR;
                const float ez = uz * innerR;

                // Inner dough face should stay flat at halfH.
                // The extra crust height is provided only by rounded crust caps.
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(0.0f, -halfH, 0.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(0.0f, halfH, 0.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(ex, halfH, ez);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(ex, -halfH, ez);
                glEnd();

                // Rounded crust cap profile — matches the outer crust ring.
                for (int j = 0; j < g_edgeSegments; ++j)
                {
                    const float phi0 = (j / static_cast<float>(g_edgeSegments)) * PI - PI * 0.5f;
                    const float phi1 = ((j + 1) / static_cast<float>(g_edgeSegments)) * PI - PI * 0.5f;

                    const float r0 = innerR + cos(phi0) * crustTubeR;
                    const float cy0 = tubeCenterY + sin(phi0) * crustTubeR;
                    const float r1 = innerR + cos(phi1) * crustTubeR;
                    const float cy1 = tubeCenterY + sin(phi1) * crustTubeR;

                    glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, j / static_cast<float>(g_edgeSegments));
                    glVertex3f(ex, cy0, ez);
                    glTexCoord2f(0.0f, (j + 1) / static_cast<float>(g_edgeSegments));
                    glVertex3f(ex, cy1, ez);
                    glTexCoord2f(1.0f, (j + 1) / static_cast<float>(g_edgeSegments));
                    glVertex3f(ux * r1, cy1, uz * r1);
                    glTexCoord2f(1.0f, j / static_cast<float>(g_edgeSegments));
                    glVertex3f(ux * r0, cy0, uz * r0);
                    glEnd();
                }
            }
        }
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
}
