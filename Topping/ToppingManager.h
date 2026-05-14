#pragma once
#include "../Math/Vec3.h"
#include "../Scene/SceneObject.h"
#include <random>
#include <vector>

class ToppingManager {
public:
    ToppingManager(float pizzaRadius, float pizzaHeight, float crustEdgeRadius, int sliceCount = 1);

    void addCheeseBatch();
    void removeCheeseBatch();
    void addPepperoni();
    void removePepperoni();
    void addPeasBatch();
    void removePeasBatch();
    void addPineappleBatch();
    void removePineappleBatch();
    void addSauce();
    void removeSauce();
    void setSliceCount(int sliceCount);
    void setPizzaHeight(float pizzaHeight);
    // Called every frame to propagate per-slice slide offsets.
    void syncSliceOffsets(const float offsets[16]);

    /// When baked: cheese rotations become flat (0). When unbaked: restores saved rotations.
    void syncCheeseForBakeState(bool baked);

    const std::vector<SceneObject>& getCheese() const { return cheese_; }
    const std::vector<SceneObject>& getPepperoni() const { return pepperoni_; }
    const std::vector<SceneObject>& getPeas() const { return peas_; }
    const std::vector<SceneObject>& getPineapple() const { return pineapple_; }
    const std::vector<SceneObject>& getSauce() const { return sauce_; }

    ~ToppingManager();

    ToppingManager(const ToppingManager&) = delete;
    ToppingManager& operator=(const ToppingManager&) = delete;

private:
    float innerRadius_;
    float pizzaHalfH_;
    int sliceCount_;
    std::mt19937 rng_;
    std::vector<SceneObject> cheese_;
    std::vector<Vec3> cheeseUnbakedRotation_;
    std::vector<float> cheeseBaseX_, cheeseBaseZ_;
    std::vector<SceneObject> pepperoni_;
    std::vector<float> pepperoniBaseX_, pepperoniBaseZ_;
    std::vector<SceneObject> peas_;
    std::vector<float> peasBaseX_, peasBaseZ_;
    std::vector<SceneObject> pineapple_;
    std::vector<float> pineappleBaseX_, pineappleBaseZ_;
    std::vector<SceneObject> sauce_;

    float surfaceYForToppings() const;

    bool pepperoniOverlaps(float x, float z, float r) const;
    bool tryPlacePepperoni(float& outX, float& outZ, float r);
    bool tryPlacePineapple(float& outX, float& outZ, float footprintR);
};
