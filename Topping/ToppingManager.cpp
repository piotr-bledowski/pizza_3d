#include "ToppingManager.h"
#include "../Mesh/CheeseCuboid.h"
#include "../Mesh/Pea.h"
#include "../Mesh/Pepperoni.h"
#include "../Mesh/PineappleRing.h"
#include "../Mesh/RedOnionWedge.h"
#include "../Mesh/Sauce.h"
#include "../Texture/TextureManager.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace {
constexpr int kCheesePerClick = 50;
constexpr int kPeasPerClick = 10;
constexpr float kPeaRadiusMin = 0.028f;
constexpr float kPeaRadiusMax = 0.055f;
constexpr int kPeaSphereSlices = 10;
constexpr int kPeaSphereStacks = 10;
constexpr int kPepperoniMaxAttempts = 80;
constexpr int kPepperoniPerClick = 10;
constexpr int kPineappleMaxAttempts = 80;
constexpr int kPineapplePerClick = 10;
constexpr float kPepperoniRadiusMin = 0.135f;
constexpr float kPepperoniRadiusMax = 0.21f;
constexpr float kPepperoniHeightMin = 0.015f;
constexpr float kPepperoniHeightMax = 0.030f;
constexpr float kCheeseDimMin = 0.035f;
constexpr float kCheeseDimMax = 0.095f;
constexpr float kPlacementMargin = 0.04f;
constexpr float kSauceLayerHeight = 0.012f;
constexpr float kSauceRadiusMargin = 0.10f;
constexpr float kToppingAboveSauce = 0.003f;
constexpr float kToppingEdgeInset = 0.05f;
// Fixed-size pineapple rings (all pieces identical). ~1.5× prior footprint.
constexpr float kPineappleOuterRadius = 0.315f;
constexpr float kPineappleInnerRadius = 0.117f;
constexpr float kPineappleThickness = 0.02175f;
constexpr int kPineappleRingSegs = 32;
constexpr int kRedOnionPerClick = 10;
constexpr int kRedOnionMaxAttempts = 80;
constexpr int kRedOnionArcSegs = 10;
constexpr float PI = 3.14159265358979323846f;
} // namespace

ToppingManager::ToppingManager(float pizzaRadius, float pizzaHeight, float crustEdgeRadius, int sliceCount)
    : innerRadius_(std::max(0.01f, pizzaRadius - crustEdgeRadius))
    , pizzaHalfH_(pizzaHeight * 0.5f)
    , sliceCount_(std::max(1, sliceCount))
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
    for (auto& o : pineapple_) {
        delete o.mesh;
    }
    for (auto& o : redOnion_) {
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
    for (const auto& o : pineapple_) {
        const float dx = x - o.position.x;
        const float dz = z - o.position.z;
        const float dist = std::sqrt(dx * dx + dz * dz);
        const auto* ring = static_cast<const PineappleRing*>(o.mesh);
        const float otherR = ring ? ring->outerRadius : kPineappleOuterRadius;
        if (dist < r + otherR + kPlacementMargin) {
            return true;
        }
    }
    for (const auto& o : redOnion_) {
        const float dx = x - o.position.x;
        const float dz = z - o.position.z;
        const float dist = std::sqrt(dx * dx + dz * dz);
        const auto* wedge = static_cast<const RedOnionWedge*>(o.mesh);
        const float otherR = wedge ? wedge->outerRadius : 0.12f;
        if (dist < r + otherR + kPlacementMargin) {
            return true;
        }
    }
    return false;
}

bool ToppingManager::tryPlacePepperoni(float& outX, float& outZ, float r) {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    const float maxR = std::max(0.05f, innerRadius_ - r - kPlacementMargin - kToppingEdgeInset);
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

bool ToppingManager::tryPlacePineapple(float& outX, float& outZ, float footprintR) {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    const float maxR = std::max(0.05f, innerRadius_ - footprintR - kPlacementMargin - kToppingEdgeInset);
    for (int attempt = 0; attempt < kPineappleMaxAttempts; ++attempt) {
        const float t = u01(rng_) * 2.0f * PI;
        const float rad = maxR * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);
        if (!pepperoniOverlaps(x, z, footprintR)) {
            outX = x;
            outZ = z;
            return true;
        }
    }
    return false;
}

bool ToppingManager::tryPlaceRedOnion(float& outX, float& outZ, float footprintR) {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    const float maxR = std::max(0.05f, innerRadius_ - footprintR - kPlacementMargin - kToppingEdgeInset);
    for (int attempt = 0; attempt < kRedOnionMaxAttempts; ++attempt) {
        const float t = u01(rng_) * 2.0f * PI;
        const float rad = maxR * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);
        if (!pepperoniOverlaps(x, z, footprintR)) {
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
        const float rad = std::max(0.01f, innerRadius_ - kToppingEdgeInset) * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);

        const float lift = 0.5f * std::max(w, std::max(h, d));
        const float y = surfaceYForToppings() + lift;

        const Vec3 unbakedRot{rotDist(rng_), rotDist(rng_), rotDist(rng_)};

        SceneObject obj{};
        obj.mesh = new CheeseCuboid(w, h, d);
        obj.position = {x, y, z};
        if (TextureManager::isBaked()) {
            obj.rotation = {0.0f, 0.0f, 0.0f};
        } else {
            obj.rotation = unbakedRot;
        }
        cheeseUnbakedRotation_.push_back(unbakedRot);
        cheeseBaseX_.push_back(x);
        cheeseBaseZ_.push_back(z);
        cheese_.push_back(obj);
    }
}

void ToppingManager::removeCheeseBatch() {
    for (int k = 0; k < kCheesePerClick && !cheese_.empty(); ++k) {
        delete cheese_.back().mesh;
        cheese_.pop_back();
        cheeseUnbakedRotation_.pop_back();
        if (!cheeseBaseX_.empty()) { cheeseBaseX_.pop_back(); cheeseBaseZ_.pop_back(); }
    }
}

void ToppingManager::syncCheeseForBakeState(bool baked) {
    const size_t n = cheese_.size();
    if (cheeseUnbakedRotation_.size() != n) {
        return;
    }
    for (size_t i = 0; i < n; ++i) {
        if (baked) {
            cheese_[i].rotation = {0.0f, 0.0f, 0.0f};
        } else {
            cheese_[i].rotation = cheeseUnbakedRotation_[i];
        }
    }
}

void ToppingManager::addPepperoni() {
    std::uniform_real_distribution<float> radDist(kPepperoniRadiusMin, kPepperoniRadiusMax);
    std::uniform_real_distribution<float> hDist(kPepperoniHeightMin, kPepperoniHeightMax);
    std::uniform_real_distribution<float> rotY(-180.0f, 180.0f);

    for (int n = 0; n < kPepperoniPerClick; ++n) {
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
        pepperoniBaseX_.push_back(px);
        pepperoniBaseZ_.push_back(pz);
        pepperoni_.push_back(obj);
    }
}

void ToppingManager::removePepperoni() {
    for (int k = 0; k < kPepperoniPerClick && !pepperoni_.empty(); ++k) {
        delete pepperoni_.back().mesh;
        pepperoni_.pop_back();
        if (!pepperoniBaseX_.empty()) {
            pepperoniBaseX_.pop_back();
            pepperoniBaseZ_.pop_back();
        }
    }
}

void ToppingManager::addPeasBatch() {
    std::uniform_real_distribution<float> u01(0.0f, 1.0f);
    std::uniform_real_distribution<float> radDist(kPeaRadiusMin, kPeaRadiusMax);
    std::uniform_real_distribution<float> rotDist(-180.0f, 180.0f);

    for (int i = 0; i < kPeasPerClick; ++i) {
        const float pr = radDist(rng_);
        const float t = u01(rng_) * 2.0f * PI;
        const float rad = std::max(0.01f, innerRadius_ - pr - kToppingEdgeInset) * std::sqrt(u01(rng_));
        const float x = rad * std::cos(t);
        const float z = rad * std::sin(t);
        const float y = surfaceYForToppings() + pr;

        SceneObject obj{};
        obj.mesh = new Pea(pr, kPeaSphereSlices, kPeaSphereStacks);
        obj.position = {x, y, z};
        obj.rotation = {rotDist(rng_), rotDist(rng_), rotDist(rng_)};
        peasBaseX_.push_back(x);
        peasBaseZ_.push_back(z);
        peas_.push_back(obj);
    }
}

void ToppingManager::removePeasBatch() {
    for (int k = 0; k < kPeasPerClick && !peas_.empty(); ++k) {
        delete peas_.back().mesh;
        peas_.pop_back();
        if (!peasBaseX_.empty()) { peasBaseX_.pop_back(); peasBaseZ_.pop_back(); }
    }
}

void ToppingManager::addPineappleBatch() {
    const float outerR = kPineappleOuterRadius;
    const float innerR = kPineappleInnerRadius;
    const float thick = kPineappleThickness;

    for (int i = 0; i < kPineapplePerClick; ++i) {
        float px = 0.0f;
        float pz = 0.0f;
        if (!tryPlacePineapple(px, pz, outerR)) {
            return;
        }

        SceneObject obj{};
        obj.mesh = new PineappleRing(outerR, innerR, thick, kPineappleRingSegs);
        const float lift = thick * 0.5f;
        obj.position = {px, surfaceYForToppings() + lift, pz};
        obj.rotation = {0.0f, 0.0f, 0.0f};
        pineappleBaseX_.push_back(px);
        pineappleBaseZ_.push_back(pz);
        pineapple_.push_back(obj);
    }
}

void ToppingManager::removePineappleBatch() {
    for (int k = 0; k < kPineapplePerClick && !pineapple_.empty(); ++k) {
        delete pineapple_.back().mesh;
        pineapple_.pop_back();
        if (!pineappleBaseX_.empty()) {
            pineappleBaseX_.pop_back();
            pineappleBaseZ_.pop_back();
        }
    }
}

void ToppingManager::addRedOnionBatch() {
    // Tube geometry: outerR - innerR is the tube diameter (thin strip), so keep it small.
    // outerR and innerR are measured from the piece's local origin (placed randomly on pizza).
    std::uniform_real_distribution<float> outerDist(0.095f, 0.125f);
    std::uniform_real_distribution<float> stripWidthDist(0.010f, 0.015f); // radial width (thin)
    std::uniform_real_distribution<float> wedgeDist(1.10f, 1.55f);       // arc span in radians (long crescent)
    std::uniform_real_distribution<float> thickDist(0.040f, 0.058f);     // vertical height (tall, flattened cross-section)

    for (int i = 0; i < kRedOnionPerClick; ++i) {
        const float outerR = outerDist(rng_);
        const float innerR = outerR - stripWidthDist(rng_);
        const float wedge = wedgeDist(rng_);
        const float thick = thickDist(rng_);

        float px = 0.0f;
        float pz = 0.0f;
        if (!tryPlaceRedOnion(px, pz, outerR)) {
            return;
        }

        SceneObject obj{};
        obj.mesh = new RedOnionWedge(outerR, innerR, wedge, thick, kRedOnionArcSegs);
        const float lift = thick * 0.5f;
        obj.position = {px, surfaceYForToppings() + lift, pz};
        obj.rotation = {0.0f, std::atan2(px, pz) * (180.0f / PI), 0.0f};
        redOnionBaseX_.push_back(px);
        redOnionBaseZ_.push_back(pz);
        redOnion_.push_back(obj);
    }
}

void ToppingManager::removeRedOnionBatch() {
    for (int k = 0; k < kRedOnionPerClick && !redOnion_.empty(); ++k) {
        delete redOnion_.back().mesh;
        redOnion_.pop_back();
        if (!redOnionBaseX_.empty()) {
            redOnionBaseX_.pop_back();
            redOnionBaseZ_.pop_back();
        }
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
    for (auto& o : pineapple_) {
        o.position.y += delta;
    }
    for (auto& o : redOnion_) {
        o.position.y += delta;
    }

    const float r = std::max(0.1f, innerRadius_ - kSauceRadiusMargin);
    SceneObject obj{};
    obj.mesh = new Sauce(r, kSauceLayerHeight, 32, sliceCount_);
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
    for (auto& o : pineapple_) {
        o.position.y -= delta;
    }
    for (auto& o : redOnion_) {
        o.position.y -= delta;
    }

    delete sauce_.back().mesh;
    sauce_.pop_back();
}

void ToppingManager::setSliceCount(int sliceCount) {
    sliceCount_ = std::max(1, sliceCount);
    for (auto& o : sauce_) {
        if (auto* sauceMesh = dynamic_cast<Sauce*>(o.mesh)) {
            sauceMesh->sliceCount = sliceCount_;
        }
    }
}

void ToppingManager::syncSliceOffsets(const float offsets[16]) {
    // Propagate offsets to the Sauce mesh draw loop.
    for (auto& o : sauce_) {
        if (auto* s = dynamic_cast<Sauce*>(o.mesh)) {
            for (int i = 0; i < 16; ++i) {
                s->sliceOffsets[i] = offsets[i];
            }
        }
    }

    if (sliceCount_ <= 1) {
        return; // no slicing active, toppings stay put
    }

    const float sliceAngle = 2.0f * PI / static_cast<float>(sliceCount_);

    auto applyOffsets = [&](std::vector<SceneObject>& objs,
                            const std::vector<float>& bx,
                            const std::vector<float>& bz) {
        for (size_t i = 0; i < objs.size() && i < bx.size(); ++i) {
            float theta = std::atan2(bz[i], bx[i]);
            if (theta < 0.0f) theta += 2.0f * PI;
            const int idx = std::min(sliceCount_ - 1, (int)(theta / sliceAngle));
            const float midAngle = (idx + 0.5f) * sliceAngle;
            const float slideAmt = (idx < 16) ? offsets[idx] : 0.0f;
            objs[i].position.x = bx[i] + std::cos(midAngle) * slideAmt;
            objs[i].position.z = bz[i] + std::sin(midAngle) * slideAmt;
        }
    };

    applyOffsets(cheese_, cheeseBaseX_, cheeseBaseZ_);
    applyOffsets(pepperoni_, pepperoniBaseX_, pepperoniBaseZ_);
    applyOffsets(peas_, peasBaseX_, peasBaseZ_);
    applyOffsets(pineapple_, pineappleBaseX_, pineappleBaseZ_);
    applyOffsets(redOnion_, redOnionBaseX_, redOnionBaseZ_);
}

void ToppingManager::setPizzaHeight(float pizzaHeight) {
    const float newHalfH = pizzaHeight * 0.5f;
    const float deltaY = newHalfH - pizzaHalfH_;
    if (std::abs(deltaY) < 1e-6f) {
        return;
    }

    pizzaHalfH_ = newHalfH;

    for (auto& o : sauce_) {
        o.position.y += deltaY;
    }
    for (auto& o : cheese_) {
        o.position.y += deltaY;
    }
    for (auto& o : pepperoni_) {
        o.position.y += deltaY;
    }
    for (auto& o : peas_) {
        o.position.y += deltaY;
    }
    for (auto& o : pineapple_) {
        o.position.y += deltaY;
    }
    for (auto& o : redOnion_) {
        o.position.y += deltaY;
    }
}
