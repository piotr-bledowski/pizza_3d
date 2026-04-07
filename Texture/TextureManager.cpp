#include "TextureManager.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

unsigned int TextureManager::g_pizzaTextureID = 0;

void TextureManager::generatePizzaTexture()
{
    constexpr int width = 256;
    constexpr int height = 256;
    const int n = width * height;

    // Warm pizza dough base (wheat / pale baked dough)
    const float baseR = 0.82f;
    const float baseG = 0.74f;
    const float baseB = 0.62f;

    std::vector<float> rCh(n, baseR);
    std::vector<float> gCh(n, baseG);
    std::vector<float> bCh(n, baseB);

    // Low-contrast noise (~3x coarser than the very fine speckle pass)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int i = y * width + x;
            const float nx = static_cast<float>(x) / static_cast<float>(width);
            const float ny = static_cast<float>(y) / static_cast<float>(height);
            const float grain = 0.0055f * std::sin(nx * 42.3f + ny * 36.3f) + 0.0042f * std::sin(nx * 80.3f - ny * 65.7f) + 0.0032f * std::sin(nx * 127.7f + ny * 103.7f) + 0.0025f * std::sin(nx * 173.7f - ny * 139.7f);
            rCh[i] += grain;
            gCh[i] += grain * 0.96f;
            bCh[i] += grain * 0.9f;
        }
    }

    // Soft air-bubble specks (~3x larger radius than the previous fine pass)
    std::mt19937 rng(0x50495A41u);
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    std::uniform_int_distribution<int> lightOrDark(0, 1);

    constexpr int kNumBubbles = 80;
    for (int b = 0; b < kNumBubbles; ++b)
    {
        const float cx = u01(rng) * static_cast<float>(width);
        const float cy = u01(rng) * static_cast<float>(height);
        const float rad = 1.95f + u01(rng) * 9.6f;
        const float radSq = rad * rad;

        const bool lighter = lightOrDark(rng) == 1;
        const float amp = 0.01f + u01(rng) * 0.022f;
        float dr = 0.0f;
        float dg = 0.0f;
        float db = 0.0f;
        if (lighter)
        {
            dr = amp * (0.9f + u01(rng) * 0.15f);
            dg = amp * (0.85f + u01(rng) * 0.12f);
            db = amp * (0.75f + u01(rng) * 0.1f);
        }
        else
        {
            dr = -amp * (0.85f + u01(rng) * 0.2f);
            dg = -amp * (0.8f + u01(rng) * 0.18f);
            db = -amp * (0.7f + u01(rng) * 0.15f);
        }

        const int x0 = std::max(0, static_cast<int>(std::floor(cx - rad - 1.0f)));
        const int x1 = std::min(width - 1, static_cast<int>(std::ceil(cx + rad + 1.0f)));
        const int y0 = std::max(0, static_cast<int>(std::floor(cy - rad - 1.0f)));
        const int y1 = std::min(height - 1, static_cast<int>(std::ceil(cy + rad + 1.0f)));

        for (int yy = y0; yy <= y1; ++yy)
        {
            for (int xx = x0; xx <= x1; ++xx)
            {
                const float dx = static_cast<float>(xx) - cx;
                const float dy = static_cast<float>(yy) - cy;
                const float d2 = dx * dx + dy * dy;
                if (d2 >= radSq)
                {
                    continue;
                }
                const float d = std::sqrt(d2);
                const float t = 1.0f - (d / rad);
                const float w = t * t * (3.0f - 2.0f * t);

                const int idx = yy * width + xx;
                rCh[idx] += dr * w;
                gCh[idx] += dg * w;
                bCh[idx] += db * w;
            }
        }
    }

    std::vector<unsigned char> textureData(static_cast<size_t>(width * height * 3));
    for (int i = 0; i < n; ++i)
    {
        const float rf = std::clamp(rCh[i], 0.0f, 1.0f);
        const float gf = std::clamp(gCh[i], 0.0f, 1.0f);
        const float bf = std::clamp(bCh[i], 0.0f, 1.0f);
        textureData[static_cast<size_t>(i * 3 + 0)] = static_cast<unsigned char>(rf * 255.0f + 0.5f);
        textureData[static_cast<size_t>(i * 3 + 1)] = static_cast<unsigned char>(gf * 255.0f + 0.5f);
        textureData[static_cast<size_t>(i * 3 + 2)] = static_cast<unsigned char>(bf * 255.0f + 0.5f);
    }

    glGenTextures(1, &g_pizzaTextureID);
    glBindTexture(GL_TEXTURE_2D, g_pizzaTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
}

void TextureManager::initPizzaTexture()
{
    generatePizzaTexture();
}

void TextureManager::bindPizzaTexture()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_pizzaTextureID);
}

void TextureManager::cleanupTextures()
{
    if (g_pizzaTextureID != 0)
    {
        glDeleteTextures(1, &g_pizzaTextureID);
        g_pizzaTextureID = 0;
    }
}
