#include <GL/freeglut.h>
#include <vector>

#include "Renderer.h"
#include "Input.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Cube.h"
#include "Math/Vec3.h"

// User-controlled parameters
float g_radius = 2.5f;
float g_height = 0.3f;
int   g_segments = 32;

float g_cubeSize = 0.5f;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scene");

    // Scene definition
    std::vector<Mesh*> meshes;
    std::vector<Vec3> positions;

    // Cylinder
    meshes.push_back(new Cylinder(g_radius, g_height, g_segments));
    positions.push_back({ 0.0f, 0.0f, 0.0f });

    // Cube above cylinder
    meshes.push_back(new Cube(g_cubeSize));
    positions.push_back({ 0.0f, g_height * 0.5f + g_cubeSize, 0.0f });

    // Send to renderer
    setScene(meshes, positions);

    initGL();

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}