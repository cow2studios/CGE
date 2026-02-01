#pragma once
#include "../Node2D.h"

enum class BillboardMode {
    DISABLED,       // Lying flat (e.g., a rug or shadow)
    Y_AXIS,         // Rotates only on Y (like Doom sprites or trees)
    ALL_AXIS        // Always faces camera (standard particle/sprite look)
};

class SpriteNode : public Node2D {
private:
    SDL_GPUTexture* texture = nullptr;
    float width = 0;
    float height = 0;

public:
    // 2.5D Specific Features
    BillboardMode billboard_mode = BillboardMode::Y_AXIS; // Default to Y-Axis for characters
    bool is_lit = true; // Should 3D lights affect this sprite?

    // Anchoring: (0.5, 1.0) places the sprite's "feet" at the position
    float anchor_x = 0.5f;
    float anchor_y = 1.0f;

    // Visuals
    float modulate_r = 1.0f, modulate_g = 1.0f, modulate_b = 1.0f, modulate_a = 1.0f;
    bool flip_h = false;
    bool flip_v = false;

    SpriteNode(const std::string& name);

    void SetTexture(SDL_GPUTexture* tex);
    void Render(SDL_GPURenderPass* pass) override;
};