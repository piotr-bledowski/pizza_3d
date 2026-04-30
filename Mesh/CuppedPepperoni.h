#pragma once
#include "Mesh.h"

class CuppedPepperoni : public Mesh {
public:
    float radius;
    float thickness;
    int segments;

    CuppedPepperoni(float r, float t, int s);
    void draw() override;
};
