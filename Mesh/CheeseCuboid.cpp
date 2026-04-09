#include "CheeseCuboid.h"
#include "FlattenedSphere.h"
#include <GL/freeglut.h>
#include <algorithm>

CheeseCuboid::CheeseCuboid(float w, float h, float d)
    : Cuboid(w, h, d)
{
    // Isotropic horizontal spread: same radius on X and Z so the melt spreads evenly in every
    // direction (circular footprint). Scale from the larger cuboid face so small sides still melt out.
    constexpr float kHorizontalSpread = 2.65f;
    const float r = std::max(w, d) * 0.5f * kHorizontalSpread;
    const float rx = r;
    const float rz = r;
    const float ry = std::max(0.016f, std::min(h * 0.30f, 0.11f));
    meltedCheese_ = std::make_unique<FlattenedSphere>(rx, ry, rz);
}

void CheeseCuboid::draw()
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.95f, 0.9f, 0.45f);
    Cuboid::draw();
    glColor3f(1.0f, 1.0f, 1.0f);
}

Mesh* CheeseCuboid::bakeSubstitute()
{
    return meltedCheese_.get();
}
