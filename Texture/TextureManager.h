#pragma once

class TextureManager {
public:
    static void initPizzaTexture();
    static void initSauceTexture();
    static void initPepperoniTexture();
    static void initCheeseTexture();
    static void initPineappleTexture();
    static void initRedOnionTexture();
    static void bindPizzaTexture();
    static void bindSauceTexture();
    static void bindPepperoniTexture();
    static void bindCheeseTexture();
    static void bindPineappleTexture();
    static void bindRedOnionTexture();
    static void cleanupTextures();

    static void setBaked(bool baked);
    static bool isBaked();

private:
    static unsigned int g_pizzaTextureID;
    static unsigned int g_sauceTextureID;
    static unsigned int g_pepperoniTextureID;
    static unsigned int g_cheeseTextureID;
    static unsigned int g_pineappleTextureID;
    static unsigned int g_redOnionTextureID;
    static bool g_baked;

    static void generatePizzaTexture();
    static void generateSauceTexture();
    static void generatePepperoniTexture();
    static void generateCheeseTexture();
    static void generatePineappleTexture();
    static void generateRedOnionTexture();
    static void regenerateAllTextures();
};
