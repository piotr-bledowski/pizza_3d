#pragma once
#include <vector>

struct SceneObject;

void initGL();
void renderScene();

// Scene interface
void setScene(const std::vector<SceneObject>& objects);