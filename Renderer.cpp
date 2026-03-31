#include "Renderer.h"
#include "Camera.h"
#include "Mesh/Mesh.h"
#include "Math/Vec3.h"
#include <GL/freeglut.h>
#include <vector>

// Internal scene storage
static std::vector<Mesh*> g_meshes;
static std::vector<Vec3>  g_positions;

void setScene(const std::vector<Mesh*>& meshes,
    const std::vector<Vec3>& positions)
{
    g_meshes = meshes;
    g_positions = positions;
}

void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
    updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    applyCameraView();

    // Render all meshes
    for (size_t i = 0; i < g_meshes.size(); ++i) {
        glPushMatrix();

        if (i < g_positions.size()) {
            const Vec3& p = g_positions[i];
            glTranslatef(p.x, p.y, p.z);
        }

        g_meshes[i]->draw();

        glPopMatrix();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}