#include "UI.h"
#include "../Input.h"
#include <GL/freeglut.h>
#include <string.h>
#include <vector>

namespace
{

    struct UIText
    {
        float nx, ny;
        char text[512];
        Color4 color;
    };

    struct UIButton
    {
        float nx, ny, nw, nh;
        char label[256];
        Color4 textColor, bgColor, borderColor;
        float borderThickness;
        int id;
    };

    std::vector<UIText> g_frameTexts;
    std::vector<UIButton> g_frameButtons;
    std::vector<UIButton> g_lastButtons;

    int g_nextButtonId = 0;
    int g_pendingClickId = -1;

    void *kFont = GLUT_BITMAP_9_BY_15;

    int textWidthPixels(const char *s)
    {
        int w = 0;
        for (const char *p = s; *p; ++p)
        {
            w += glutBitmapWidth(kFont, static_cast<unsigned char>(*p));
        }
        return w;
    }

    void drawTextPixels(int px, int py, const char *text, const Color4 &color)
    {
        glColor4f(color.r, color.g, color.b, color.a);
        glRasterPos2i(px, py);
        for (const char *p = text; *p; ++p)
        {
            glutBitmapCharacter(kFont, static_cast<unsigned char>(*p));
        }
    }

} // namespace

void uiBeginFrame()
{
    g_frameTexts.clear();
    g_frameButtons.clear();
    g_nextButtonId = 0;
}

void uiEndFrameClicks()
{
    if (g_pendingClickId < 0)
    {
        return;
    }
    bool found = false;
    for (const UIButton &b : g_frameButtons)
    {
        if (b.id == g_pendingClickId)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        g_pendingClickId = -1;
    }
}

void drawText(float nx, float ny, const char *text, const Color4 &color)
{
    UIText t{};
    t.nx = nx;
    t.ny = ny;
    t.color = color;
    (void)strncpy_s(t.text, sizeof(t.text), text, _TRUNCATE);
    g_frameTexts.push_back(t);
}

bool drawButton(float nx, float ny, float nw, float nh, const char *label,
                const Color4 &textColor, const Color4 &bgColor,
                float borderThickness, const Color4 &borderColor)
{
    UIButton b{};
    b.nx = nx;
    b.ny = ny;
    b.nw = nw;
    b.nh = nh;
    b.textColor = textColor;
    b.bgColor = bgColor;
    b.borderColor = borderColor;
    b.borderThickness = borderThickness;
    b.id = g_nextButtonId++;
    (void)strncpy_s(b.label, sizeof(b.label), label, _TRUNCATE);
    g_frameButtons.push_back(b);

    bool hit = (g_pendingClickId == b.id);
    if (hit)
    {
        g_pendingClickId = -1;
    }
    return hit;
}

void uiOnMouseClick(int x, int y, int button, int state)
{
    if (getControlMode() != ControlMode::UI)
    {
        return;
    }
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
    {
        return;
    }
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    if (w <= 0 || h <= 0)
    {
        return;
    }

    for (int i = static_cast<int>(g_lastButtons.size()) - 1; i >= 0; --i)
    {
        const UIButton &b = g_lastButtons[static_cast<size_t>(i)];
        float px = b.nx * static_cast<float>(w);
        float py = b.ny * static_cast<float>(h);
        float pw = b.nw * static_cast<float>(w);
        float ph = b.nh * static_cast<float>(h);
        if (static_cast<float>(x) >= px && static_cast<float>(x) < px + pw && static_cast<float>(y) >= py && static_cast<float>(y) < py + ph)
        {
            g_pendingClickId = b.id;
            return;
        }
    }
}

void uiRenderOverlay(int windowWidth, int windowHeight)
{
    if (windowWidth <= 0 || windowHeight <= 0)
    {
        return;
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(windowWidth), static_cast<double>(windowHeight), 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    for (const UIText &t : g_frameTexts)
    {
        int px = static_cast<int>(t.nx * windowWidth);
        int py = static_cast<int>(t.ny * windowHeight);
        drawTextPixels(px, py, t.text, t.color);
    }

    for (const UIButton &b : g_frameButtons)
    {
        float px = b.nx * static_cast<float>(windowWidth);
        float py = b.ny * static_cast<float>(windowHeight);
        float pw = b.nw * static_cast<float>(windowWidth);
        float ph = b.nh * static_cast<float>(windowHeight);

        glColor4f(b.bgColor.r, b.bgColor.g, b.bgColor.b, b.bgColor.a);
        glBegin(GL_QUADS);
        glVertex2f(px, py);
        glVertex2f(px + pw, py);
        glVertex2f(px + pw, py + ph);
        glVertex2f(px, py + ph);
        glEnd();

        glLineWidth(b.borderThickness);
        glColor4f(b.borderColor.r, b.borderColor.g, b.borderColor.b, b.borderColor.a);
        glBegin(GL_LINE_LOOP);
        glVertex2f(px + 0.5f, py + 0.5f);
        glVertex2f(px + pw - 0.5f, py + 0.5f);
        glVertex2f(px + pw - 0.5f, py + ph - 0.5f);
        glVertex2f(px + 0.5f, py + ph - 0.5f);
        glEnd();

        int tw = textWidthPixels(b.label);
        int tx = static_cast<int>(px + (pw - static_cast<float>(tw)) * 0.5f);
        int ty = static_cast<int>(py + (ph - 15.0f) * 0.5f + 12.0f);
        drawTextPixels(tx, ty, b.label, b.textColor);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();

    g_lastButtons = g_frameButtons;
}
