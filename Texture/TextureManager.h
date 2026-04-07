#pragma once

class TextureManager {
public:
    static void initPizzaTexture();
    static void initSauceTexture();
    static void bindPizzaTexture();
    static void bindSauceTexture();
    static void cleanupTextures();

private:
    static unsigned int g_pizzaTextureID;
    static unsigned int g_sauceTextureID;
    static void generatePizzaTexture();
    static void generateSauceTexture();
};
