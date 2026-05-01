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
    const float innerR = radius - edgeR;
    const int slices = sliceCount > 1 ? sliceCount : 1;
    const float sliceAngle = 2.0f * PI / static_cast<float>(slices);
    const float gap = slices > 1 ? 0.012f : 0.0f;
    const int segmentsPerSlice = std::max(2, segments / slices);

    for (int slice = 0; slice < slices; ++slice)
    {
        const float start = slice * sliceAngle + gap;
        const float end = (slice + 1) * sliceAngle - gap;
        const float step = (end - start) / static_cast<float>(segmentsPerSlice);

        // Top face
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, halfH, 0.0f);
        for (int i = 0; i <= segmentsPerSlice; ++i)
        {
            float theta = start + i * step;
            float u = 0.5f + 0.5f * cos(theta);
            float v = 0.5f + 0.5f * sin(theta);
            glTexCoord2f(u, v);
            glVertex3f(cos(theta) * innerR, halfH, sin(theta) * innerR);
        }
        glEnd();

        // Bottom face
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -halfH, 0.0f);
        for (int i = segmentsPerSlice; i >= 0; --i)
        {
            float theta = start + i * step;
            float u = 0.5f + 0.5f * cos(theta);
            float v = 0.5f + 0.5f * sin(theta);
            glTexCoord2f(u, v);
            glVertex3f(cos(theta) * innerR, -halfH, sin(theta) * innerR);
        }
        glEnd();

        // Edge (crust)
        for (int j = 0; j < g_edgeSegments; ++j)
        {
            float phi0 = (j / (float)g_edgeSegments) * PI - PI / 2.0f;
            float phi1 = ((j + 1) / (float)g_edgeSegments) * PI - PI / 2.0f;

            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i <= segmentsPerSlice; ++i)
            {
                float theta = start + i * step;

                float cx = cos(theta) * innerR;
                float cz = sin(theta) * innerR;

                float x0 = cx + cos(theta) * cos(phi0) * edgeR;
                float y0 = sin(phi0) * edgeR;
                float z0 = cz + sin(theta) * cos(phi0) * edgeR;

                float x1 = cx + cos(theta) * cos(phi1) * edgeR;
                float y1 = sin(phi1) * edgeR;
                float z1 = cz + sin(theta) * cos(phi1) * edgeR;

                glTexCoord2f((float)i / segmentsPerSlice, 0.3f + 0.7f * (j / (float)g_edgeSegments));
                glVertex3f(x0, y0, z0);
                glTexCoord2f((float)i / segmentsPerSlice, 0.3f + 0.7f * ((j + 1) / (float)g_edgeSegments));
                glVertex3f(x1, y1, z1);
            }
            glEnd();
        }

        if (slices > 1)
        {
            for (int side = 0; side < 2; ++side)
            {
                const float theta = (side == 0) ? start : end;
                const float ux = cos(theta);
                const float uz = sin(theta);
                const float edgeX = ux * innerR;
                const float edgeZ = uz * innerR;

                // Flat radial face from center to inner crust radius.
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(0.0f, -halfH, 0.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(0.0f, halfH, 0.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(edgeX, halfH, edgeZ);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(edgeX, -halfH, edgeZ);
                glEnd();

                // Rounded crust cap so slice edges match unsliced crust profile.
                for (int j = 0; j < g_edgeSegments; ++j)
                {
                    const float phi0 = (j / static_cast<float>(g_edgeSegments)) * PI - PI * 0.5f;
                    const float phi1 = ((j + 1) / static_cast<float>(g_edgeSegments)) * PI - PI * 0.5f;

                    const float r0 = innerR + cos(phi0) * edgeR;
                    const float y0 = sin(phi0) * edgeR;
                    const float r1 = innerR + cos(phi1) * edgeR;
                    const float y1 = sin(phi1) * edgeR;

                    glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, j / static_cast<float>(g_edgeSegments));
                    glVertex3f(edgeX, y0, edgeZ);
                    glTexCoord2f(0.0f, (j + 1) / static_cast<float>(g_edgeSegments));
                    glVertex3f(edgeX, y1, edgeZ);
                    glTexCoord2f(1.0f, (j + 1) / static_cast<float>(g_edgeSegments));
                    glVertex3f(ux * r1, y1, uz * r1);
                    glTexCoord2f(1.0f, j / static_cast<float>(g_edgeSegments));
                    glVertex3f(ux * r0, y0, uz * r0);
                    glEnd();
                }
            }
        }
    }

    glDisable(GL_TEXTURE_2D);
}