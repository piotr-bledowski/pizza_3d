#pragma once

class TextureManager {
public:
    static void initPizzaTexture();
    static void bindPizzaTexture();
    static void cleanupTextures();

private:
    static unsigned int g_pizzaTextureID;
    static void generatePizzaTexture();
};
