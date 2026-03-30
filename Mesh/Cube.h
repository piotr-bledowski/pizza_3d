#pragma once
#include "Mesh.h"

class Cube : public Mesh {
public:
    float size;

    Cube(float s);
    void draw() override;
};