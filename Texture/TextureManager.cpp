#include "TextureManager.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

unsigned int TextureManager::g_pizzaTextureID = 0;
unsigned int TextureManager::g_sauceTextureID = 0;
unsigned int TextureManager::g_pepperoniTextureID = 0;

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

void TextureManager::generateSauceTexture()
{
    constexpr int width = 256;
    constexpr int height = 256;
    const int n = width * height;

    // Tomato base: red-dominant, tight range (all shades stay "sauce red")
    const float baseR = 0.66f;
    const float baseG = 0.085f;
    const float baseB = 0.055f;

    std::vector<float> rCh(n, baseR);
    std::vector<float> gCh(n, baseG);
    std::vector<float> bCh(n, baseB);

    // Smeared streaks: sums of sin(nx*a + ny*b) = parallel bands / brush strokes
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int i = y * width + x;
            const float nx = static_cast<float>(x) / static_cast<float>(width);
            const float ny = static_cast<float>(y) / static_cast<float>(height);

            const float s1 = std::sin(nx * 24.0f + ny * 18.0f);
            const float s2 = std::sin(nx * 11.5f - ny * 28.0f);
            const float s3 = std::sin(nx * 33.0f + ny * 9.0f);
            const float s4 = 0.65f * std::sin(nx * 71.0f + ny * 52.0f);
            const float s5 = 0.5f * std::sin(nx * 5.0f + ny * 4.2f);

            float streak = 0.026f * s1 + 0.022f * s2 + 0.02f * s3 + 0.014f * s4;
            const float slow = 0.5f + 0.5f * std::sin(nx * 2.1f + ny * 1.7f);
            streak *= (0.58f + 0.42f * slow);
            streak += 0.012f * s5;

            // Stay in red family: mostly R, slight G/B shift with streak
            rCh[i] += streak * 1.15f;
            gCh[i] += streak * 0.32f;
            bCh[i] += streak * 0.22f;
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

    glGenTextures(1, &g_sauceTextureID);
    glBindTexture(GL_TEXTURE_2D, g_sauceTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
}

void TextureManager::generatePepperoniTexture()
{
    constexpr int width = 256;
    constexpr int height = 256;
    const int n = width * height;

    // Pepperoni base: deep cured-meat red with subtle variation.
    const float baseR = 0.42f;
    const float baseG = 0.10f;
    const float baseB = 0.085f;

    std::vector<float> rCh(n, baseR);
    std::vector<float> gCh(n, baseG);
    std::vector<float> bCh(n, baseB);

    // Gentle marbling so the slice isn't flat.
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int i = y * width + x;
            const float nx = static_cast<float>(x) / static_cast<float>(width);
            const float ny = static_cast<float>(y) / static_cast<float>(height);
            const float marbling = 0.018f * std::sin(nx * 18.0f + ny * 24.0f) + 0.012f * std::sin(nx * 43.0f - ny * 31.0f);
            rCh[i] += marbling;
            gCh[i] += marbling * 0.42f;
            bCh[i] += marbling * 0.36f;
        }
    }

    // Small fat specks (salami-like): off-white/pink dots.
    std::mt19937 rng(0x50455050u);
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    constexpr int kNumSpecks = 260;
    for (int s = 0; s < kNumSpecks; ++s)
    {
        const float cx = u01(rng) * static_cast<float>(width);
        const float cy = u01(rng) * static_cast<float>(height);
        const float rx = 0.65f + u01(rng) * 1.8f;
        const float ry = 0.55f + u01(rng) * 1.6f;
        const float angle = u01(rng) * 6.28318530718f;
        const float ca = std::cos(angle);
        const float sa = std::sin(angle);

        const int x0 = std::max(0, static_cast<int>(std::floor(cx - rx - 1.0f)));
        const int x1 = std::min(width - 1, static_cast<int>(std::ceil(cx + rx + 1.0f)));
        const int y0 = std::max(0, static_cast<int>(std::floor(cy - ry - 1.0f)));
        const int y1 = std::min(height - 1, static_cast<int>(std::ceil(cy + ry + 1.0f)));

        for (int yy = y0; yy <= y1; ++yy)
        {
            for (int xx = x0; xx <= x1; ++xx)
            {
                const float dx = static_cast<float>(xx) - cx;
                const float dy = static_cast<float>(yy) - cy;
                const float qx = (dx * ca + dy * sa) / rx;
                const float qy = (-dx * sa + dy * ca) / ry;
                const float d2 = qx * qx + qy * qy;
                if (d2 >= 1.0f)
                {
                    continue;
                }
                const float t = 1.0f - std::sqrt(d2);
                const float w = t * t * (3.0f - 2.0f * t);
                const int idx = yy * width + xx;
                rCh[idx] += 0.18f * w;
                gCh[idx] += 0.13f * w;
                bCh[idx] += 0.11f * w;
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

    glGenTextures(1, &g_pepperoniTextureID);
    glBindTexture(GL_TEXTURE_2D, g_pepperoniTextureID);

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

void TextureManager::initSauceTexture()
{
    generateSauceTexture();
}

void TextureManager::initPepperoniTexture()
{
    generatePepperoniTexture();
}

void TextureManager::bindPizzaTexture()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_pizzaTextureID);
}

void TextureManager::bindSauceTexture()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_sauceTextureID);
}

void TextureManager::bindPepperoniTexture()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_pepperoniTextureID);
}

void TextureManager::cleanupTextures()
{
    if (g_pizzaTextureID != 0)
    {
        glDeleteTextures(1, &g_pizzaTextureID);
        g_pizzaTextureID = 0;
    }
    if (g_sauceTextureID != 0)
    {
        glDeleteTextures(1, &g_sauceTextureID);
        g_sauceTextureID = 0;
    }
    if (g_pepperoniTextureID != 0)
    {
        glDeleteTextures(1, &g_pepperoniTextureID);
        g_pepperoniTextureID = 0;
    }
}
