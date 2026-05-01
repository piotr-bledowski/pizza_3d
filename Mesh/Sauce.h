#pragma once
#include "Mesh.h"

class Sauce : public Mesh {
public:
    float radius;
    float height;
    int segments;
    int sliceCount;
    int hoveredSlice = -1;
    float hoverLightBoost = 0.15f;
    // Per-slice radial slide offset (world units). Index == slice index.
    float sliceOffsets[16] = {};

    Sauce(float r, float h, int s, int slices = 1);
    void draw() override;
};
