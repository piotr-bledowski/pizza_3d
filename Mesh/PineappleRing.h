#pragma once
#include "Mesh.h"

/// Flat canned-pineapple ring (full annulus), +Y up through the thickness.
class PineappleRing : public Mesh {
public:
    float outerRadius;
    float innerRadius;
    float thickness;
    int ringSegments;

    PineappleRing(float outerR, float innerR, float thick, int ringSegs);
    void draw() override;

private:
    static constexpr float kBakedScaleXZ = 0.95f;
    static constexpr float kBakedScaleY = 0.96f;
    static constexpr float kBakedColorR = 0.94f;
    static constexpr float kBakedColorG = 0.91f;
    static constexpr float kBakedColorB = 0.82f;
};
