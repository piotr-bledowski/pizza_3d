#pragma once
#include <vector>

class Mesh;
struct Vec3;

void initGL();
void renderScene();

// Scene interface
void setScene(const std::vector<Mesh*>& meshes,
    const std::vector<Vec3>& positions);