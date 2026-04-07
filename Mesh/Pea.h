#pragma once
#include "Sphere.h"

class Pea : public Sphere {
public:
    Pea(float r, int sl, int st);
    void draw() override;
};
