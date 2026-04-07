#pragma once
#include "Cuboid.h"

class CheeseCuboid : public Cuboid {
public:
    CheeseCuboid(float w, float h, float d);
    void draw() override;
};
