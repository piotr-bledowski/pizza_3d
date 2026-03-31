#pragma once
#include <vector>
#include "SceneObject.h"

class SceneManager {
public:
    void addObject(const SceneObject& obj);
    void removeLastObject();

    const std::vector<SceneObject>& getObjects() const;

private:
    std::vector<SceneObject> objects;
};