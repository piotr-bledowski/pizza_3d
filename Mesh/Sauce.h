#pragma once
#include "Mesh.h"

class Sauce : public Mesh {
public:
    float radius;
    float height;
    int segments;
    int sliceCount;

    Sauce(float r, float h, int s, int slices = 1);
    void draw() override;
};
