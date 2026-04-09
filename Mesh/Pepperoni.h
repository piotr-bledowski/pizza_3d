#pragma once
#include "Mesh.h"
#include "CuppedPepperoni.h"
#include <memory>

class Pepperoni : public Mesh {
public:
    float radius;
    float height;
    int segments;

    Pepperoni(float r, float h, int s);
    void draw() override;
    Mesh* bakeSubstitute() override;

private:
    std::unique_ptr<CuppedPepperoni> cupped_;
};
