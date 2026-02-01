#pragma once
#include "../Node.h"

class Node2D : public Node {
public:
    // 2.5D Transform Data
    float position_x = 0.0f;
    float position_y = 0.0f;
    float position_z = 0.0f; // Critical for 2.5D depth sorting and mixing with 3D

    float rotation = 0.0f;   // Z-axis rotation (standard 2D spin)

    float scale_x = 1.0f;
    float scale_y = 1.0f;

    // Helper: Is this node visible in the 3D world?
    bool visible = true;

    Node2D(const std::string& name);

    // Future: Helper to get a full 4x4 Model Matrix for the shader
    // void GetModelMatrix(float* matrix_out);
};