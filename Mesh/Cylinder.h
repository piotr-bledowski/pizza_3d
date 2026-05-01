#pragma once
#include "Mesh.h"

class Cylinder : public Mesh {
public:
    float radius;
    float height;
    int segments;
    int sliceCount;
    float crustPuff = 0.0f;

    Cylinder(float r, float h, int s, int slices = 1);
    void draw() override;
};