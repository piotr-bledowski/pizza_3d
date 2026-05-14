#pragma once
#include "Mesh.h"

/// Thin annular sector — crescent slice of red onion (julienne-style strip bent on an arc), +Y up, bisector +Z.
class RedOnionWedge : public Mesh {
public:
    float outerRadius;
    float innerRadius;
    float wedgeAngle;
    float thickness;
    int arcSegments;

    RedOnionWedge(float outerR, float innerR, float wedgeRad, float thick, int arcSegs);
    void draw() override;

private:
    static constexpr float kBakedScaleXZ = 0.88f;
    static constexpr float kBakedScaleY  = 0.90f;
    static constexpr float kBakedColorR  = 0.85f;
    static constexpr float kBakedColorG  = 0.78f;
    static constexpr float kBakedColorB  = 0.84f;
};
