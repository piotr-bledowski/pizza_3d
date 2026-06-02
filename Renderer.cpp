#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "Scene/SceneObject.h"
#include "Mesh/Mesh.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Sauce.h"
#include "Texture/TextureManager.h"
#include "UI/UI.h"
#include <GL/freeglut.h>
#include <cmath>
#include <vector>

// Internal scene storage
static std::vector<SceneObject> g_objects;

namespace {
constexpr float PI = 3.14159265358979323846f;

// Raycasts cursor to pizza top plane and returns hovered slice index.
int computeHoveredSlice(const Cylinder& pizza, int mouseX, int mouseY, int winW, int winH)
{
    if (pizza.sliceCount <= 1 || winW <= 0 || winH <= 0)
    {
        return -1;
    }

    GLdouble model[16];
    GLdouble proj[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, viewport);

    const GLdouble sx = static_cast<GLdouble>(mouseX);
    const GLdouble sy = static_cast<GLdouble>(winH - mouseY);
    GLdouble ox, oy, oz;
    GLdouble fx, fy, fz;
    if (gluUnProject(sx, sy, 0.0, model, proj, viewport, &ox, &oy, &oz) != GL_TRUE)
    {
        return -1;
    }
    if (gluUnProject(sx, sy, 1.0, model, proj, viewport, &fx, &fy, &fz) != GL_TRUE)
    {
        return -1;
    }

    const double dx = fx - ox;
    const double dy = fy - oy;
    const double dz = fz - oz;
    if (std::abs(dy) < 1e-6)
    {
        return -1;
    }

    // Pizza lives at y=0 in world space.
    const double t = -oy / dy;
    if (t <= 0.0)
    {
        return -1;
    }

    const double hx = ox + dx * t;
    const double hz = oz + dz * t;
    const double dist = std::sqrt(hx * hx + hz * hz);
    if (dist > static_cast<double>(pizza.radius))
    {
        return -1;
    }

    double theta = std::atan2(hz, hx);
    if (theta < 0.0)
    {
        theta += 2.0 * PI;
    }

    const int slices = pizza.sliceCount;
    const double sliceAngle = (2.0 * PI) / static_cast<double>(slices);
    const double gap = 0.012;
    const int idx = static_cast<int>(theta / sliceAngle);
    const double start = static_cast<double>(idx) * sliceAngle + gap;
    const double end = static_cast<double>(idx + 1) * sliceAngle - gap;
    if (theta < start || theta > end)
    {
        return -1;
    }
    return idx;
}
} // namespace

void setScene(const std::vector<SceneObject> &objects)
{
    g_objects = objects;
}

void initGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);

    // Initialize all textures once at startup.
    TextureManager::initPizzaTexture();
    TextureManager::initSauceTexture();
    TextureManager::initPepperoniTexture();
    TextureManager::initCheeseTexture();
    TextureManager::initPineappleTexture();
    TextureManager::initRedOnionTexture();

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    // Allow glColor to tint textured fragments (used for hover highlighting).
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

static int s_hoveredSlice = -1;

int getHoveredSlice()
{
    return s_hoveredSlice;
}

void renderScene()
{
    // --- 3D prep ----------------------------------------------------------
    // Camera movement/orientation are updated every rendered frame.
    updateCamera();

    // Clear buffers and reset world->view transform.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply camera view transform (gluLookAt).
    applyCameraView();

    // --- Hover slice detection (UI mode only) -----------------------------
    // In UI mode we convert the mouse position to a pizza-slice index
    // so slice clicks can toggle "pulled out" animation.
    // In UI mode, mouse cursor can point to slices for highlight/click actions.
    if (getControlMode() == ControlMode::UI)
    {
        int mx = 0;
        int my = 0;
        getMousePosition(mx, my);
        const int winW = glutGet(GLUT_WINDOW_WIDTH);
        const int winH = glutGet(GLUT_WINDOW_HEIGHT);
        for (const auto& obj : g_objects)
        {
            if (const auto* cyl = dynamic_cast<Cylinder*>(obj.mesh))
            {
                s_hoveredSlice = computeHoveredSlice(*cyl, mx, my, winW, winH);
                break;
            }
        }
    }
    else
    {
        s_hoveredSlice = -1;
    }

    // --- Draw loop --------------------------------------------------------
    // Each object is drawn with its position/rotation, and in baked mode
    // we may swap to a pre-baked substitute mesh via `bakeSubstitute()`.
    for (const auto &obj : g_objects)
    {
        if (!obj.mesh)
            continue;

        glPushMatrix();

        glTranslatef(obj.position.x, obj.position.y, obj.position.z);
        glRotatef(obj.rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(obj.rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(obj.rotation.z, 0.0f, 0.0f, 1.0f);

        Mesh* drawMesh = obj.mesh;
        if (TextureManager::isBaked())
        {
            if (Mesh* sub = obj.mesh->bakeSubstitute())
            {
                drawMesh = sub;
            }
        }

        if (auto* c = dynamic_cast<Cylinder*>(drawMesh))
        {
            // Hover highlight: only cylinders/sauces know how to interpret slices.
            c->hoveredSlice = (c->sliceCount > 1) ? s_hoveredSlice : -1;
        }
        if (auto* s = dynamic_cast<Sauce*>(drawMesh))
        {
            s->hoveredSlice = (s->sliceCount > 1) ? s_hoveredSlice : -1;
        }
        drawMesh->draw();

        glPopMatrix();
    }

    // --- 2D overlay -------------------------------------------------------
    // Draw buttons/labels on top of the 3D scene, then present frame.
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    uiRenderOverlay(winW, winH);

    // Present + ensure we keep animating (slice animation depends on tickSliceAnimation).
    glutSwapBuffers();
    glutPostRedisplay();
}
