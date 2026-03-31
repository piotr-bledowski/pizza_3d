#pragma once
#include "Mesh.h"

class TriangularPrism : public Mesh {
public:
    float baseWidth;
    float triangleHeight;
    float depth;

    TriangularPrism(float baseW, float triH, float d);
    void draw() override;
};
