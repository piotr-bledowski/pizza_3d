#pragma once
#include "Mesh.h"

class Sauce : public Mesh {
public:
    float radius;
    float height;
    int segments;

    Sauce(float r, float h, int s);
    void draw() override;
};
