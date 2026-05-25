#include <GL/freeglut.h>
#include <algorithm>
#include <cstdio>
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
#include "Texture/TextureManager.h"
#include "UI/UI.h"
#include "UI/Localization.h"

float g_radius = 2.5f;
float g_height = 0.15f;
int g_segments = 32;
float g_cubeSize = 0.5f;
float g_cuboidWidth = 0.4f;
float g_cuboidHeight = 0.2f;
float g_cuboidDepth = 0.6f;

constexpr float g_pizzaCrustEdgeRadius = 0.15f;
constexpr int g_defaultPizzaSliceCount = 6;
const float g_bakedPizzaHeight = g_height;
const float g_rawPizzaHeight = g_height * 0.5f;

SceneManager g_scene;
ToppingManager g_toppings(g_radius, g_rawPizzaHeight, g_pizzaCrustEdgeRadius, 1);
Cylinder *g_pizzaBaseMesh = nullptr;
bool g_showToppingControls = false;
bool g_isSliced = false;
int g_sliceCount = g_defaultPizzaSliceCount;
int g_cheeseClicks = 0;
int g_pepperoniClicks = 0;
int g_peasClicks = 0;
int g_pineappleClicks = 0;
int g_redOnionClicks = 0;

// Slice pull-out state
constexpr float kSlideDistance = 0.55f;
constexpr float kSlideSpeed = 0.14f;
static bool g_sliceSelected[16] = {};
static float g_sliceCurrentOffset[16] = {};

static void buildUI()
{
    Color4 text{1.0f, 1.0f, 1.0f, 1.0f};
    Color4 label{0.92f, 0.92f, 0.94f, 1.0f};
    Color4 bg{0.22f, 0.22f, 0.26f, 0.96f};
    Color4 darkLabelBg{0.15f, 0.15f, 0.18f, 0.98f};
    Color4 border{0.45f, 0.5f, 0.55f, 1.0f};
    const float bt = 2.0f;

    const float bw = 0.2f;
    const float bh = 0.08f;

    drawText(0.02f, 0.02f, uiStr(UiString::HintTab), text);
    drawText(0.02f, 0.055f, uiStr(UiString::HintCamera), text);

    const float langBtnW = 0.12f;
    const float langBtnH = 0.055f;
    const float langBtnX = 1.0f - langBtnW - 0.02f;
    const float langBtnY = 0.02f;
    const UiString langLabel = (getUiLanguage() == UiLanguage::English)
        ? UiString::LangSwitchToPolish
        : UiString::LangSwitchToEnglish;
    if (drawButton(langBtnX, langBtnY, langBtnW, langBtnH, uiStr(langLabel), label, bg, bt, border))
    {
        toggleUiLanguage();
    }

    if (getControlMode() == ControlMode::UI)
    {
        drawText(0.02f, 0.09f, uiStr(UiString::HintUiMode), text);

        const float leftX = 0.02f;
        const float buttonGap = 0.02f;
        const float bakeY = 0.90f;
        const float sliceY = bakeY - bh - 0.02f;
        const float manageY = sliceY - bh - 0.02f;
        const float manageX = leftX;
        const float manageW = 0.42f;
        const float sliceBtnW = bw;
        const float smallW = 0.055f;
        const float smallGap = 0.01f;

        const char *manageLabel = g_showToppingControls
            ? uiStr(UiString::ToppingManageHide)
            : uiStr(UiString::ToppingManageShow);
        if (drawButton(manageX, manageY, manageW, bh, manageLabel, label, bg, bt, border))
        {
            g_showToppingControls = !g_showToppingControls;
        }

        if (drawButton(leftX, sliceY, sliceBtnW, bh, uiStr(UiString::Slice), label, bg, bt, border))
        {
            g_isSliced = true;
        }
        const float unsliceX = leftX + sliceBtnW + buttonGap;
        if (drawButton(unsliceX, sliceY, sliceBtnW, bh, uiStr(UiString::Unslice), label, bg, bt, border))
        {
            g_isSliced = false;
            for (int i = 0; i < 16; ++i)
                g_sliceSelected[i] = false;
        }
        const float sliceCountX = unsliceX + sliceBtnW + buttonGap;
        if (drawButton(sliceCountX, sliceY, smallW, bh, "-", label, bg, bt, border))
        {
            g_sliceCount = std::max(2, g_sliceCount - 1);
        }
        if (drawButton(sliceCountX + smallW + smallGap, sliceY, smallW, bh, "+", label, bg, bt, border))
        {
            g_sliceCount = std::min(16, g_sliceCount + 1);
        }
        char sliceCountBuffer[32];
        snprintf(sliceCountBuffer, sizeof(sliceCountBuffer), uiStr(UiString::SlicesCountFmt), g_sliceCount);
        drawText(sliceCountX + smallW + smallGap + smallW + 0.01f, sliceY + 0.05f, sliceCountBuffer, text);

        const int effectiveSliceCount = g_isSliced ? g_sliceCount : 1;
        if (g_pizzaBaseMesh)
        {
            g_pizzaBaseMesh->sliceCount = effectiveSliceCount;
        }
        g_toppings.setSliceCount(effectiveSliceCount);

        if (drawButton(leftX, bakeY, bw, bh, uiStr(UiString::Bake), label, bg, bt, border))
        {
            TextureManager::setBaked(true);
            g_toppings.syncCheeseForBakeState(true);
            if (g_pizzaBaseMesh)
            {
                g_pizzaBaseMesh->height = g_bakedPizzaHeight;
                g_pizzaBaseMesh->crustPuff = 1.0f;
            }
            g_toppings.setPizzaHeight(g_bakedPizzaHeight);
        }

        if (drawButton(leftX + bw + buttonGap, bakeY, bw, bh, uiStr(UiString::Unbake), label, bg, bt, border))
        {
            TextureManager::setBaked(false);
            g_toppings.syncCheeseForBakeState(false);
            if (g_pizzaBaseMesh)
            {
                g_pizzaBaseMesh->height = g_rawPizzaHeight;
                g_pizzaBaseMesh->crustPuff = 0.0f;
            }
            g_toppings.setPizzaHeight(g_rawPizzaHeight);
        }

        if (!g_showToppingControls)
        {
            return;
        }

        const float rowX = leftX;
        const float rowStartY = manageY - 0.08f;
        const float rowStep = 0.08f;
        const float rowH = 0.07f;
        const float nameW = 0.23f;
        const float counterX = rowX + nameW + smallGap + smallW + smallGap + smallW + 0.02f;
        const float counterYOffset = 0.045f;

        char counterBuffer[32];

        float y = rowStartY;

        drawLabelBox(rowX, y, nameW, rowH, uiStr(UiString::Sauce), label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addSauce();
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removeSauce();
        }

        y -= rowStep;
        drawLabelBox(rowX, y, nameW, rowH, uiStr(UiString::Cheese), label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addCheeseBatch();
            ++g_cheeseClicks;
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removeCheeseBatch();
            --g_cheeseClicks;
        }
        snprintf(counterBuffer, sizeof(counterBuffer), "%d", g_cheeseClicks);
        drawText(counterX, y + counterYOffset, counterBuffer, text);

        y -= rowStep;
        drawLabelBox(rowX, y, nameW, rowH, uiStr(UiString::Pepperoni), label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addPepperoni();
            ++g_pepperoniClicks;
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removePepperoni();
            --g_pepperoniClicks;
        }
        snprintf(counterBuffer, sizeof(counterBuffer), "%d", g_pepperoniClicks);
        drawText(counterX, y + counterYOffset, counterBuffer, text);

        y -= rowStep;
        drawLabelBox(rowX, y, nameW, rowH, uiStr(UiString::Peas), label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addPeasBatch();
            ++g_peasClicks;
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removePeasBatch();
            --g_peasClicks;
        }
        snprintf(counterBuffer, sizeof(counterBuffer), "%d", g_peasClicks);
        drawText(counterX, y + counterYOffset, counterBuffer, text);

        y -= rowStep;
        drawLabelBox(rowX, y, nameW, rowH, uiStr(UiString::Pineapple), label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addPineappleBatch();
            ++g_pineappleClicks;
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removePineappleBatch();
            --g_pineappleClicks;
        }
        snprintf(counterBuffer, sizeof(counterBuffer), "%d", g_pineappleClicks);
        drawText(counterX, y + counterYOffset, counterBuffer, text);

        y -= rowStep;
        drawLabelBox(rowX, y, nameW, rowH, uiStr(UiString::RedOnion), label, darkLabelBg, bt, border);
        if (drawButton(rowX + nameW + smallGap, y, smallW, rowH, "+", label, bg, bt, border))
        {
            g_toppings.addRedOnionBatch();
            ++g_redOnionClicks;
        }
        if (drawButton(rowX + nameW + smallGap + smallW + smallGap, y, smallW, rowH, "-", label, bg, bt, border))
        {
            g_toppings.removeRedOnionBatch();
            --g_redOnionClicks;
        }
        snprintf(counterBuffer, sizeof(counterBuffer), "%d", g_redOnionClicks);
        drawText(counterX, y + counterYOffset, counterBuffer, text);
    }
}

static void tickSliceAnimation()
{
    // Toggle selection on click if hovered over a slice.
    if (consumePizzaClick())
    {
        const int hs = getHoveredSlice();
        if (hs >= 0 && hs < 16 && g_isSliced)
        {
            g_sliceSelected[hs] = !g_sliceSelected[hs];
        }
    }

    // Smoothly animate each slice offset toward its target.
    for (int i = 0; i < 16; ++i)
    {
        const bool sel = g_isSliced && (i < g_sliceCount) && g_sliceSelected[i];
        const float target = sel ? kSlideDistance : 0.0f;
        g_sliceCurrentOffset[i] += (target - g_sliceCurrentOffset[i]) * kSlideSpeed;
        if (std::abs(g_sliceCurrentOffset[i]) < 0.001f && target == 0.0f)
        {
            g_sliceCurrentOffset[i] = 0.0f;
        }
    }

    // Push offsets into the pizza base mesh.
    if (g_pizzaBaseMesh)
    {
        for (int i = 0; i < 16; ++i)
        {
            g_pizzaBaseMesh->sliceOffsets[i] = g_sliceCurrentOffset[i];
        }
    }

    // Propagate to sauce and individual toppings.
    g_toppings.syncSliceOffsets(g_sliceCurrentOffset);
}

static void updateScene()
{
    const std::vector<SceneObject> &base = g_scene.getObjects();
    const std::vector<SceneObject> &sauce = g_toppings.getSauce();
    const std::vector<SceneObject> &cheese = g_toppings.getCheese();
    const std::vector<SceneObject> &pep = g_toppings.getPepperoni();
    const std::vector<SceneObject> &peas = g_toppings.getPeas();
    const std::vector<SceneObject> &pineapple = g_toppings.getPineapple();
    const std::vector<SceneObject> &redOnion = g_toppings.getRedOnion();

    std::vector<SceneObject> all;
    all.reserve(base.size() + sauce.size() + cheese.size() + pep.size() + peas.size() + pineapple.size() + redOnion.size());
    all.insert(all.end(), base.begin(), base.end());
    all.insert(all.end(), sauce.begin(), sauce.end());
    all.insert(all.end(), cheese.begin(), cheese.end());
    all.insert(all.end(), pep.begin(), pep.end());
    all.insert(all.end(), peas.begin(), peas.end());
    all.insert(all.end(), pineapple.begin(), pineapple.end());
    all.insert(all.end(), redOnion.begin(), redOnion.end());
    setScene(all);
}

void displayWrapper()
{
    uiBeginFrame();
    buildUI();
    uiEndFrameClicks();
    tickSliceAnimation();
    updateScene();
    renderScene();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scene");

    g_pizzaBaseMesh = new Cylinder(g_radius, g_rawPizzaHeight, g_segments, 1);
    g_scene.addObject({g_pizzaBaseMesh, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});

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
