#include "TextureManager.h"
#include <GL/freeglut.h>
#include <cstring>
#include <cmath>

unsigned int TextureManager::g_pizzaTextureID = 0;

void TextureManager::generatePizzaTexture() {
    const int width = 256;
    const int height = 256;
    unsigned char* textureData = new unsigned char[width * height * 3];

    // Generate simple pizza texture
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;

            // Base dough color (golden brown)
            unsigned char base_r = 210;
            unsigned char base_g = 180;
            unsigned char base_b = 140;

            // Add some variation using Perlin-like noise
            float noise = sinf(x * 0.1f) * cosf(y * 0.1f) * 0.5f + 0.5f;
            noise += sinf(x * 0.05f) * cosf(y * 0.03f) * 0.3f;

            base_r = (unsigned char)(base_r * (0.8f + noise * 0.2f));
            base_g = (unsigned char)(base_g * (0.8f + noise * 0.2f));
            base_b = (unsigned char)(base_b * (0.7f + noise * 0.1f));

            // Add some "cheese" spots (yellow-orange patches)
            float cheese_noise = sinf(x * 0.15f + 3.14f) * cosf(y * 0.18f + 2.0f);
            if (cheese_noise > 0.3f) {
                base_r = (unsigned char)(base_r * 1.1f);
                base_g = (unsigned char)(base_g * 0.95f);
                base_b = (unsigned char)(base_b * 0.7f);
            }

            // Add darker spots for crust/char
            float char_noise = sinf(x * 0.08f) * cosf(y * 0.12f);
            if (char_noise < -0.4f) {
                base_r = (unsigned char)(base_r * 0.7f);
                base_g = (unsigned char)(base_g * 0.6f);
                base_b = (unsigned char)(base_b * 0.5f);
            }

            textureData[idx] = base_r;
            textureData[idx + 1] = base_g;
            textureData[idx + 2] = base_b;
        }
    }

    glGenTextures(1, &g_pizzaTextureID);
    glBindTexture(GL_TEXTURE_2D, g_pizzaTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

    delete[] textureData;
}

void TextureManager::initPizzaTexture() {
    generatePizzaTexture();
}

void TextureManager::bindPizzaTexture() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_pizzaTextureID);
}

void TextureManager::cleanupTextures() {
    if (g_pizzaTextureID != 0) {
        glDeleteTextures(1, &g_pizzaTextureID);
        g_pizzaTextureID = 0;
    }
}
