#include "ToppingManager.h"
#include "../Mesh/CheeseCuboid.h"
#include "../Mesh/Pea.h"
#include "../Mesh/Pepperoni.h"
#include "../Mesh/Sauce.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace {
constexpr int kCheesePerClick = 10;
constexpr int kPeasPerClick = 10;
constexpr float kPeaRadiusMin = 0.028f;
constexpr float kPeaRadiusMax = 0.055f;
constexpr int kPeaSphereSlices = 10;
constexpr int kPeaSphereStacks = 10;
constexpr int kPepperoniMaxAttempts = 80;
constexpr float kPepperoniRadiusMin = 0.09f;
constexpr float kPepperoniRadiusMax = 0.14f;
constexpr float kPepperoniHeightMin = 0.018f;
constexpr float kPepperoniHeightMax = 0.035f;
constexpr float kCheeseDimMin = 0.035f;
constexpr float kCheeseDimMax = 0.095f;
constexpr float kPlacementMargin = 0.04f;
constexpr float kSauceLayerHeight = 0.012f;
constexpr float kSauceRadiusMargin = 0.08f;
constexpr float kToppingAboveSauce = 0.003f;
constexpr float PI = 3.14159265358979323846f;
} // namespace

ToppingManager::ToppingManager(float pizzaRadius, float pizzaHeight, float crustEdgeRadius)
    : innerRadius_(std::max(0.01f, pizzaRadius - crustEdgeRadius))
    , pizzaHalfH_(pizzaHeight * 0.5f)
    , rng_(std::random_device{}()) {}

ToppingManager::~ToppingManager() {
    for (auto& o : cheese_) {
        delete o.mesh;
    }
    for (auto& o : pepperoni_) {
        delete o.mesh;
    }
    for (auto& o : peas_) {
        delete o.mesh;
    }
    for (auto& o : sauce_) {
        delete o.mesh;
    }
}

float ToppingManager::surfaceYForToppings() const {
    if (!sauce_.empty()) {
        return pizzaHalfH_ + kSauceLayerHeight + kToppingAboveSauce;
    }
    return pizzaHalfH_;
}

bool ToppingManager::pepperoniOverlaps(float x, float z, float r) const {
    for (const auto& o : pepperoni_) {
        const float dx = x - o.position.x;
        const float dz = z - o.position.z;
        const float dist = std::sqrt(dx * dx + dz * dz);
        const auto* disc = static_cast<Pepperoni*>(o.mesh);
        const float otherR = disc ? disc->radius : kPepperoniRadiusMax;
        if (dist < r + otherR + kPlacementMargin) {
            return true;
        }
    }
    return false;
}

bool ToppingManager::tryPlacePepperoni(float& outX, float& outZ, float r) {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    const float maxR = std::max(0.05f, innerRadius_ - r - kPlacementMargin);
    for (int attempt = 0; attempt < kPepperoniMaxAttempts; ++attempt) {
        const float t = u01(rng_) * 2.0f * PI;
        const float rad = maxR * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);
        if (!pepperoniOverlaps(x, z, r)) {
            outX = x;
            outZ = z;
            return true;
        }
    }
    return false;
}

void ToppingManager::addCheeseBatch() {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    std::uniform_real_distribution<float> dimDist(kCheeseDimMin, kCheeseDimMax);
    std::uniform_real_distribution<float> rotDist(-180.0f, 180.0f);

    for (int i = 0; i < kCheesePerClick; ++i) {
        const float w = dimDist(rng_);
        const float h = dimDist(rng_);
        const float d = dimDist(rng_);

        const float t = u01(rng_) * 2.0f * PI;
        const float rad = std::max(0.01f, innerRadius_ - 0.02f) * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);

        const float lift = 0.5f * std::max(w, std::max(h, d));
        const float y = surfaceYForToppings() + lift;

        SceneObject obj{};
        obj.mesh = new CheeseCuboid(w, h, d);
        obj.position = {x, y, z};
        obj.rotation = {rotDist(rng_), rotDist(rng_), rotDist(rng_)};
        cheese_.push_back(obj);
    }
}

void ToppingManager::removeCheeseBatch() {
    for (int k = 0; k < kCheesePerClick && !cheese_.empty(); ++k) {
        delete cheese_.back().mesh;
        cheese_.pop_back();
    }
}

void ToppingManager::addPepperoni() {
    std::uniform_real_distribution<float> radDist(kPepperoniRadiusMin, kPepperoniRadiusMax);
    std::uniform_real_distribution<float> hDist(kPepperoniHeightMin, kPepperoniHeightMax);
    std::uniform_real_distribution<float> rotY(-180.0f, 180.0f);

    const float pr = radDist(rng_);
    float px = 0.0f;
    float pz = 0.0f;
    if (!tryPlacePepperoni(px, pz, pr)) {
        return;
    }

    const float ph = hDist(rng_);
    const int segs = 20;

    SceneObject obj{};
    obj.mesh = new Pepperoni(pr, ph, segs);
    const float lift = ph * 0.5f;
    obj.position = {px, surfaceYForToppings() + lift, pz};
    obj.rotation = {0.0f, rotY(rng_), 0.0f};
    pepperoni_.push_back(obj);
}

void ToppingManager::removePepperoni() {
    if (pepperoni_.empty()) {
        return;
    }
    delete pepperoni_.back().mesh;
    pepperoni_.pop_back();
}

void ToppingManager::addPeasBatch() {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    std::uniform_real_distribution<float> radDist(kPeaRadiusMin, kPeaRadiusMax);
    std::uniform_real_distribution<float> rotDist(-180.0f, 180.0f);

    for (int i = 0; i < kPeasPerClick; ++i) {
        const float pr = radDist(rng_);
        const float t = u01(rng_) * 2.0f * PI;
        const float rad = std::max(0.01f, innerRadius_ - pr - 0.02f) * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);
        const float y = surfaceYForToppings() + pr;

        SceneObject obj{};
        obj.mesh = new Pea(pr, kPeaSphereSlices, kPeaSphereStacks);
        obj.position = {x, y, z};
        obj.rotation = {rotDist(rng_), rotDist(rng_), rotDist(rng_)};
        peas_.push_back(obj);
    }
}

void ToppingManager::removePeasBatch() {
    for (int k = 0; k < kPeasPerClick && !peas_.empty(); ++k) {
        delete peas_.back().mesh;
        peas_.pop_back();
    }
}

void ToppingManager::addSauce() {
    if (!sauce_.empty()) {
        return;
    }
    const float delta = kSauceLayerHeight + kToppingAboveSauce;
    for (auto& o : cheese_) {
        o.position.y += delta;
    }
    for (auto& o : pepperoni_) {
        o.position.y += delta;
    }
    for (auto& o : peas_) {
        o.position.y += delta;
    }

    const float r = std::max(0.1f, innerRadius_ - kSauceRadiusMargin);
    SceneObject obj{};
    obj.mesh = new Sauce(r, kSauceLayerHeight, 32);
    obj.position = {0.0f, pizzaHalfH_ + kSauceLayerHeight * 0.5f, 0.0f};
    obj.rotation = {0.0f, 0.0f, 0.0f};
    sauce_.push_back(obj);
}

void ToppingManager::removeSauce() {
    if (sauce_.empty()) {
        return;
    }
    const float delta = kSauceLayerHeight + kToppingAboveSauce;
    for (auto& o : cheese_) {
        o.position.y -= delta;
    }
    for (auto& o : pepperoni_) {
        o.position.y -= delta;
    }
    for (auto& o : peas_) {
        o.position.y -= delta;
    }

    delete sauce_.back().mesh;
    sauce_.pop_back();
}
