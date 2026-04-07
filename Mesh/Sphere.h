#pragma once
#include "Mesh.h"

class Sphere : public Mesh {
public:
    float radius;
    int slices;
    int stacks;

    Sphere(float r, int sl, int st);
    void draw() override;
};
