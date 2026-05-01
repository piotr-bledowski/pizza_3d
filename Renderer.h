#pragma once
#include <vector>

struct SceneObject;

void initGL();
void renderScene();

// Scene interface
void setScene(const std::vector<SceneObject>& objects);

// Returns the pizza slice index currently under the cursor (-1 if none).
int getHoveredSlice();