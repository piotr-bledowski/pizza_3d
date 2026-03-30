#pragma once
#include "Mesh.h"

class Cylinder : public Mesh {
public:
    float radius;
    float height;
    int segments;

    Cylinder(float r, float h, int s);
    void draw() override;
};