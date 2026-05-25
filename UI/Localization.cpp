#include "Localization.h"

namespace {

UiLanguage g_language = UiLanguage::English;

struct Table {
    const char* en;
    const char* pl;
};

constexpr Table kStrings[] = {
    {"Tab: switch camera / UI", "Tab: kamera / UI"},
    {"Camera: WASD move, QE up/down, mouse look (cursor kept in window)",
     "Kamera: WASD ruch, QE gora/dol, mysz (kursor zostaje w oknie)"},
    {"UI: toppings, sauce, Bake / Unbake pizza",
     "UI: dodatki, sos, pieczenie pizzy"},
    {"Show topping management", "Pokaz dodatki"},
    {"Hide topping management", "Ukryj dodatki"},
    {"Slice", "Pokroj"},
    {"Unslice", "Cofnij"},
    {"Slices: %d", "Kawalki: %d"},
    {"Bake", "Piecz"},
    {"Unbake", "Surowa"},
    {"Sauce", "Sos"},
    {"Cheese", "Ser"},
    {"Pepperoni", "Pepperoni"},
    {"Peas", "Groszek"},
    {"Pineapple", "Ananas"},
    {"Red onion", "Cebula"},
    {"PL", "PL"},
    {"EN", "EN"},
};

} // namespace

UiLanguage getUiLanguage() { return g_language; }

void setUiLanguage(UiLanguage lang) { g_language = lang; }

void toggleUiLanguage()
{
    g_language = (g_language == UiLanguage::English) ? UiLanguage::Polish : UiLanguage::English;
}

const char* uiStr(UiString id)
{
    const int idx = static_cast<int>(id);
    if (idx < 0 || idx >= static_cast<int>(sizeof(kStrings) / sizeof(kStrings[0])))
    {
        return "";
    }
    return (g_language == UiLanguage::Polish) ? kStrings[idx].pl : kStrings[idx].en;
}
