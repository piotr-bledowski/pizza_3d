#include <GL/freeglut.h>
#include <vector>

#include "Renderer.h"
#include "Input.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Cube.h"
#include "Scene/SceneManager.h"

// Parameters
float g_radius = 2.5f;
float g_height = 0.3f;
int   g_segments = 32;
float g_cubeSize = 0.5f;

// Scene system
SceneManager g_scene;

// Cube stacking state
int g_cubeCount = 0;

// Demo update loop (called every frame)
void updateScene() {
    // Add cube on 'c'
    if (isKeyPressedOnce('c')) {
        float yOffset = g_height * 0.5f + g_cubeSize + g_cubeCount * g_cubeSize * 1.2f;

        g_scene.addObject({
            new Cube(g_cubeSize),
            {0.0f, yOffset, 0.0f}
            });

        g_cubeCount++;
    }

    // Remove last cube on 'x'
    if (isKeyPressedOnce('x')) {
        if (g_cubeCount > 0) {
            g_scene.removeLastObject();
            g_cubeCount--;
        }
    }

    // Push updated scene to renderer
    setScene(g_scene.getObjects());
}

// Wrap renderer call
void displayWrapper() {
    updateScene();
    renderScene();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scene");

    // Initial object: cylinder only
    g_scene.addObject({
        new Cylinder(g_radius, g_height, g_segments),
        {0.0f, 0.0f, 0.0f}
        });

    initGL();

    glutDisplayFunc(displayWrapper);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}