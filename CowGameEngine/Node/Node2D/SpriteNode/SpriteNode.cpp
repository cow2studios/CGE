#include "SpriteNode.h"
#include <iostream>

SpriteNode::SpriteNode(const std::string& name) : Node2D(name) {}

void SpriteNode::SetTexture(SDL_GPUTexture* tex) {
    texture = tex;
    // In a full implementation, you'd query the texture size here 
    // width = (float)tex->w; height = (float)tex->h;
}

void SpriteNode::Render(SDL_GPURenderPass* pass) {
    if (!visible) return;

    if (texture) {
        // --- 2.5D RENDER LOGIC START ---

        // 1. Calculate Offset based on Anchor
        // float offset_x = -width * anchor_x;
        // float offset_y = -height * anchor_y;

        // 2. Apply Billboard Rotation
        // If (billboard_mode == Y_AXIS) {
        //    Overwite the Model Matrix rotation with the inverse of the Camera's Y rotation
        // }

        // 3. Submit to GPU
        // We aren't just drawing a rect; we are drawing a "3D Plane" at (x, y, z)
        // SDL_PushGPUVertexUniformData(pass, 0, &model_matrix, sizeof(model_matrix));
        // SDL_BindGPUTexture(pass, ...);
        // SDL_DrawGPUPrimitives(pass, ...);

        // --- 2.5D RENDER LOGIC END ---
    }

    // Render children (e.g., a sword attached to the sprite's hand)
    Node::Render(pass);
}