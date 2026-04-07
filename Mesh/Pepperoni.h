#pragma once
#include "Mesh.h"

class Pepperoni : public Mesh {
public:
    float radius;
    float height;
    int segments;

    Pepperoni(float r, float h, int s);
    void draw() override;
};
