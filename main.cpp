#include <GL/freeglut.h>
#include <vector>

#include "Renderer.h"
#include "Input.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Cube.h"
#include "Mesh/Cuboid.h"
#include "Mesh/TriangularPrism.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneObject.h"
#include "Topping/ToppingManager.h"
#include "UI/UI.h"

float g_radius = 2.5f;
float g_height = 0.3f;
int g_segments = 32;
float g_cubeSize = 0.5f;
float g_cuboidWidth = 0.4f;
float g_cuboidHeight = 0.2f;
float g_cuboidDepth = 0.6f;

constexpr float g_pizzaCrustEdgeRadius = 0.15f;

SceneManager g_scene;
ToppingManager g_toppings(g_radius, g_height, g_pizzaCrustEdgeRadius);
bool g_showToppingControls = false;

static void buildUI()
{
    Color4 text{1.0f, 1.0f, 1.0f, 1.0f};
    Color4 label{0.92f, 0.92f, 0.94f, 1.0f};
    Color4 bg{0.22f, 0.22f, 0.26f, 0.96f};
    Color4 darkLabelBg{0.15f, 0.15f, 0.18f, 0.98f};
    Color4 border{0.45f, 0.5f, 0.55f, 1.0f};
    const float bt = 2.0f;

    const float bh = 0.08f;

    drawText(0.02f, 0.02f, "Tab: switch camera / UI", text);
    drawText(0.02f, 0.055f, "Camera: WASD move, QE up/down, mouse look (cursor kept in window)", text);

    if (getControlMode() == ControlMode::UI)
    {
        drawText(0.02f, 0.09f, "UI: cheese / peas (10 per add), pepperoni (1 per add), sauce (one layer)", text);

        const float manageX = 0.02f;
        const float manageY = 0.65f;
        const float manageW = 0.42f;
        const char *manageLabel = g_showToppingControls ? "Hide topping management" : "Show topping management";
        if (drawButton(manageX, manageY, manageW, bh, manageLabel, label, bg, bt, border))
        {
            g_showToppingControls = !g_showToppingControls;
        }

        if (!g_showToppingControls)
        {
            return;
        }

        const float rowX = 0.02f;
        const float rowStartY = 0.74f;
        const float rowStep = 0.085f;
        const float rowH = 0.07f;
        const float nameW = 0.23f;
        const float smallW = 0.055f;
        const float smallGap = 0.01f;

        float y = rowStartY;

        drawLabelBox(rowX, y, nameW, rowH, "Sauce", label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addSauce();
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removeSauce();
        }

        y += rowStep;
        drawLabelBox(rowX, y, nameW, rowH, "Cheese", label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addCheeseBatch();
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removeCheeseBatch();
        }

        y += rowStep;
        drawLabelBox(rowX, y, nameW, rowH, "Pepperoni", label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addPepperoni();
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removePepperoni();
        }

        y += rowStep;
        drawLabelBox(rowX, y, nameW, rowH, "Peas", label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addPeasBatch();
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removePeasBatch();
        }
    }
}

static void updateScene()
{
    const std::vector<SceneObject> &base = g_scene.getObjects();
    const std::vector<SceneObject> &sauce = g_toppings.getSauce();
    const std::vector<SceneObject> &cheese = g_toppings.getCheese();
    const std::vector<SceneObject> &pep = g_toppings.getPepperoni();
    const std::vector<SceneObject> &peas = g_toppings.getPeas();

    std::vector<SceneObject> all;
    all.reserve(base.size() + sauce.size() + cheese.size() + pep.size() + peas.size());
    all.insert(all.end(), base.begin(), base.end());
    all.insert(all.end(), sauce.begin(), sauce.end());
    all.insert(all.end(), cheese.begin(), cheese.end());
    all.insert(all.end(), pep.begin(), pep.end());
    all.insert(all.end(), peas.begin(), peas.end());
    setScene(all);
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

    g_scene.addObject({new Cylinder(g_radius, g_height, g_segments), {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});

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
