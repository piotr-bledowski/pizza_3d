#include "SceneManager.h"

void SceneManager::addObject(const SceneObject& obj) {
    objects.push_back(obj);
}

void SceneManager::removeLastObject() {
    if (!objects.empty()) {
        delete objects.back().mesh; // cleanup
        objects.pop_back();
    }
}

const std::vector<SceneObject>& SceneManager::getObjects() const {
    return objects;
}