#pragma once
#include "Mesh.h"

/// Unit sphere scaled to an axis-aligned ellipsoid (e.g. melted cheese puddle).
class FlattenedSphere : public Mesh {
public:
    float radiusX;
    float radiusY;
    float radiusZ;
    int slices;
    int stacks;

    FlattenedSphere(float rx, float ry, float rz, int slices = 14, int stacks = 12);
    void draw() override;
};
