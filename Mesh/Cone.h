#pragma once
#include "Mesh.h"

class Cone : public Mesh {
public:
    float radius;
    float height;
    int segments;

    Cone(float r, float h, int s);
    void draw() override;
};
