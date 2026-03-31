#pragma once
#include "../Math/Vec3.h"

class Mesh;

struct SceneObject {
    Mesh* mesh;
    Vec3 position;
};