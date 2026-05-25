#pragma once

enum class UiLanguage {
    English,
    Polish,
};

enum class UiString {
    HintTab,
    HintCamera,
    HintUiMode,
    ToppingManageShow,
    ToppingManageHide,
    Slice,
    Unslice,
    SlicesCountFmt,
    Bake,
    Unbake,
    Sauce,
    Cheese,
    Pepperoni,
    Peas,
    Pineapple,
    RedOnion,
    LangSwitchToPolish,
    LangSwitchToEnglish,
};

UiLanguage getUiLanguage();
void setUiLanguage(UiLanguage lang);
void toggleUiLanguage();
const char* uiStr(UiString id);
