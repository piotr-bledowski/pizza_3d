#include "Cylinder.h"
#include <GL/freeglut.h>
#include <cmath>
#include "../Texture/TextureManager.h"


constexpr float PI = 3.14159265358979323846f;

float g_edgeRadius = 0.15f;
int   g_edgeSegments = 16;

Cylinder::Cylinder(float r, float h, int s)
    : radius(r), height(h), segments(s) {
}

void Cylinder::draw() {
    TextureManager::bindPizzaTexture();

    float halfH = height * 0.5f;
    float innerR = radius - g_edgeRadius;
    float step = 2.0f * PI / segments;

    // Top face
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
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
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
        float u = 0.5f + 0.5f * cos(theta);
        float v = 0.5f + 0.5f * sin(theta);
        glTexCoord2f(u, v);
        glVertex3f(cos(theta) * innerR, -halfH, sin(theta) * innerR);
    }
    glEnd();

    // Edge (crust)
    for (int j = 0; j < g_edgeSegments; ++j) {
        float phi0 = (j / (float)g_edgeSegments) * PI - PI / 2.0f;
        float phi1 = ((j + 1) / (float)g_edgeSegments) * PI - PI / 2.0f;

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; ++i) {
            float theta = i * step;

            float cx = cos(theta) * innerR;
            float cz = sin(theta) * innerR;

            float x0 = cx + cos(theta) * cos(phi0) * g_edgeRadius;
            float y0 = sin(phi0) * g_edgeRadius;
            float z0 = cz + sin(theta) * cos(phi0) * g_edgeRadius;

            float x1 = cx + cos(theta) * cos(phi1) * g_edgeRadius;
            float y1 = sin(phi1) * g_edgeRadius;
            float z1 = cz + sin(theta) * cos(phi1) * g_edgeRadius;

            glTexCoord2f((float)i / segments, 0.3f + 0.7f * (j / (float)g_edgeSegments));
            glVertex3f(x0, y0, z0);
            glTexCoord2f((float)i / segments, 0.3f + 0.7f * ((j + 1) / (float)g_edgeSegments));
            glVertex3f(x1, y1, z1);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}