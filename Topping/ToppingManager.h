#pragma once
#include "../Scene/SceneObject.h"
#include <random>
#include <vector>

class ToppingManager {
public:
    ToppingManager(float pizzaRadius, float pizzaHeight, float crustEdgeRadius);

    void addCheeseBatch();
    void removeCheeseBatch();
    void addPepperoni();
    void removePepperoni();
    void addPeasBatch();
    void removePeasBatch();
    void addSauce();
    void removeSauce();

    const std::vector<SceneObject>& getCheese() const { return cheese_; }
    const std::vector<SceneObject>& getPepperoni() const { return pepperoni_; }
    const std::vector<SceneObject>& getPeas() const { return peas_; }
    const std::vector<SceneObject>& getSauce() const { return sauce_; }

    ~ToppingManager();

    ToppingManager(const ToppingManager&) = delete;
    ToppingManager& operator=(const ToppingManager&) = delete;

private:
    float innerRadius_;
    float pizzaHalfH_;
    std::mt19937 rng_;
    std::vector<SceneObject> cheese_;
    std::vector<SceneObject> pepperoni_;
    std::vector<SceneObject> peas_;
    std::vector<SceneObject> sauce_;

    float surfaceYForToppings() const;

    bool pepperoniOverlaps(float x, float z, float r) const;
    bool tryPlacePepperoni(float& outX, float& outZ, float r);
};
