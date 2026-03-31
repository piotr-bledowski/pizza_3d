#pragma once

struct Color4 {
    float r, g, b, a;
};

void uiBeginFrame();
void uiEndFrameClicks();
void drawText(float nx, float ny, const char* text, const Color4& color);
bool drawButton(float nx, float ny, float nw, float nh, const char* label,
                const Color4& textColor, const Color4& bgColor,
                float borderThickness, const Color4& borderColor);
void uiRenderOverlay(int windowWidth, int windowHeight);

void uiOnMouseClick(int x, int y, int button, int state);
