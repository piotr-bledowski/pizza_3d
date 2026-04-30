#pragma once
#include "Mesh.h"

class Cylinder : public Mesh {
public:
    float radius;
    float height;
    int segments;
    int sliceCount;

    Cylinder(float r, float h, int s, int slices = 1);
    void draw() override;
};