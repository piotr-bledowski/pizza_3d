#pragma once
#include "Mesh.h"

class Cuboid : public Mesh {
public:
    float width;
    float height;
    float depth;

    Cuboid(float w, float h, float d);
    void draw() override;
};
