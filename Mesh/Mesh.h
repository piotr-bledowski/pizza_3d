#pragma once

class Mesh {
public:
    virtual void draw() = 0;

    /// When the pizza is baked, the renderer may draw this mesh instead of `draw()`.
    /// Non-owning; must outlive the primary mesh (typically owned by the same object).
    /// Return nullptr to use the normal `draw()` for this ingredient.
    virtual Mesh* bakeSubstitute() { return nullptr; }

    virtual ~Mesh() {};
};