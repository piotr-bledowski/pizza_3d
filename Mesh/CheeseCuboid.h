#pragma once
#include "Cuboid.h"
#include "FlattenedSphere.h"
#include <memory>

class CheeseCuboid : public Cuboid {
public:
    CheeseCuboid(float w, float h, float d);
    void draw() override;
    Mesh* bakeSubstitute() override;

private:
    std::unique_ptr<FlattenedSphere> meltedCheese_;
};
