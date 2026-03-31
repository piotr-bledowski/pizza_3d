#include <GL/freeglut.h>
#include "Renderer.h"
#include "Input.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Cube.h"
#include "Scene/SceneManager.h"
#include "UI/UI.h"

float g_radius = 2.5f;
float g_height = 0.3f;
int g_segments = 32;
float g_cubeSize = 0.5f;

SceneManager g_scene;
int g_cubeCount = 0;

static void buildUI()
{
    Color4 text{1.0f, 1.0f, 1.0f, 1.0f};
    Color4 label{0.92f, 0.92f, 0.94f, 1.0f};
    Color4 bg{0.22f, 0.22f, 0.26f, 0.96f};
    Color4 border{0.45f, 0.5f, 0.55f, 1.0f};
    const float bt = 2.0f;

    drawText(0.02f, 0.02f, "Tab: switch camera / UI", text);
    drawText(0.02f, 0.055f, "Camera: WASD move, QE up/down, mouse look (cursor kept in window)", text);
    drawText(0.02f, 0.09f, "UI: click Add / Remove cube", text);

    if (drawButton(0.02f, 0.88f, 0.2f, 0.08f, "Add cube", label, bg, bt, border))
    {
        float yOffset = g_height * 0.5f + g_cubeSize + g_cubeCount * g_cubeSize * 1.2f;
        g_scene.addObject({new Cube(g_cubeSize), {0.0f, yOffset, 0.0f}});
        g_cubeCount++;
    }

    if (drawButton(0.24f, 0.88f, 0.2f, 0.08f, "Remove cube", label, bg, bt, border))
    {
        if (g_cubeCount > 0)
        {
            g_scene.removeLastObject();
            g_cubeCount--;
        }
    }
}

static void updateScene()
{
    setScene(g_scene.getObjects());
}

void displayWrapper()
{
    uiBeginFrame();
    buildUI();
    uiEndFrameClicks();
    updateScene();
    renderScene();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scene");

    g_scene.addObject({new Cylinder(g_radius, g_height, g_segments), {0.0f, 0.0f, 0.0f}});

    initGL();

    glutDisplayFunc(displayWrapper);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}
