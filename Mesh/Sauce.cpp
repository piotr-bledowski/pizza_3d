#include "Sauce.h"
#include "../Texture/TextureManager.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
}

Sauce::Sauce(float r, float h, int s, int slices)
    : radius(r), height(h), segments(s), sliceCount(slices) {}

void Sauce::draw() {
    TextureManager::bindSauceTexture();
    glColor3f(1.0f, 1.0f, 1.0f);

    const float halfH = height * 0.5f;
    const int slices = sliceCount > 1 ? sliceCount : 1;
    const float sliceAngle = 2.0f * PI / static_cast<float>(slices);
    const float gap = slices > 1 ? 0.012f : 0.0f;
    const int segmentsPerSlice = std::max(2, segments / slices);

    for (int slice = 0; slice < slices; ++slice) {
        const bool isHovered = (slices > 1 && hoveredSlice == slice);
        const float shade = isHovered ? (1.0f + hoverLightBoost) : 1.0f;
        glColor3f(shade, shade, shade);

        const float start = slice * sliceAngle + gap;
        const float end = (slice + 1) * sliceAngle - gap;
        const float step = (end - start) / static_cast<float>(segmentsPerSlice);

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -halfH, 0.0f);
        for (int i = 0; i <= segmentsPerSlice; ++i) {
            const float t = start + static_cast<float>(i) * step;
            const float u = 0.5f + 0.5f * std::cos(t);
            const float v = 0.5f + 0.5f * std::sin(t);
            glTexCoord2f(u, v);
            glVertex3f(radius * std::cos(t), -halfH, radius * std::sin(t));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, halfH, 0.0f);
        for (int i = segmentsPerSlice; i >= 0; --i) {
            const float t = start + static_cast<float>(i) * step;
            const float u = 0.5f + 0.5f * std::cos(t);
            const float v = 0.5f + 0.5f * std::sin(t);
            glTexCoord2f(u, v);
            glVertex3f(radius * std::cos(t), halfH, radius * std::sin(t));
        }
        glEnd();

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segmentsPerSlice; ++i) {
            const float t = start + static_cast<float>(i) * step;
            const float x = radius * std::cos(t);
            const float z = radius * std::sin(t);
            const float u = static_cast<float>(i) / static_cast<float>(segmentsPerSlice);
            glTexCoord2f(u, 0.0f);
            glVertex3f(x, -halfH, z);
            glTexCoord2f(u, 1.0f);
            glVertex3f(x, halfH, z);
        }
        glEnd();

        if (slices > 1) {
            for (int side = 0; side < 2; ++side) {
                const float t = side == 0 ? start : end;
                const float x = radius * std::cos(t);
                const float z = radius * std::sin(t);

                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(0.0f, -halfH, 0.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(0.0f, halfH, 0.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(x, halfH, z);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(x, -halfH, z);
                glEnd();
            }
        }
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);
}
