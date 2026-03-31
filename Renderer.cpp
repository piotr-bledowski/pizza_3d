#include "Renderer.h"
#include "Camera.h"
#include "Scene/SceneObject.h"
#include "Mesh/Mesh.h"
#include "Texture/TextureManager.h"
#include "UI/UI.h"
#include <GL/freeglut.h>
#include <vector>

// Internal scene storage
static std::vector<SceneObject> g_objects;

void setScene(const std::vector<SceneObject>& objects) {
    g_objects = objects;
}

void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);

    // Initialize textures
    TextureManager::initPizzaTexture();

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
}

void renderScene() {
    updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    applyCameraView();

    // Render all objects
    for (const auto& obj : g_objects) {
        if (!obj.mesh) continue;

        glPushMatrix();

        glTranslatef(obj.position.x, obj.position.y, obj.position.z);

        obj.mesh->draw();

        glPopMatrix();
    }

    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    uiRenderOverlay(winW, winH);

    glutSwapBuffers();
    glutPostRedisplay();
}