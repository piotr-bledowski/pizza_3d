#pragma once

class TextureManager {
public:
    static void initPizzaTexture();
    static void initSauceTexture();
    static void initPepperoniTexture();
    static void bindPizzaTexture();
    static void bindSauceTexture();
    static void bindPepperoniTexture();
    static void cleanupTextures();

    static void setBaked(bool baked);
    static bool isBaked();

private:
    static unsigned int g_pizzaTextureID;
    static unsigned int g_sauceTextureID;
    static unsigned int g_pepperoniTextureID;
    static bool g_baked;

    static void generatePizzaTexture();
    static void generateSauceTexture();
    static void generatePepperoniTexture();
    static void regenerateAllTextures();
};
