#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

// Include our new specialized 2.5D Scene Graph
#include "SceneGraph.h"

// Engine State
struct EngineState {
    SDL_Window* window = nullptr;
    SDL_GPUDevice* gpu_device = nullptr;
    bool quit = false;

    // Timer
    uint64_t last_tick = 0;
    double delta_time = 0.0;

    // The Scene
    Node* root_node = nullptr;
};

// 1. App Init
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    SDL_SetAppMetadata("Cow Game Engine", "1.0", "com.cowengine.game");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("SDL Init Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    EngineState* state = new EngineState();
    *appstate = state;

    // Create GPU Device (SPIR-V for cross-platform support)
    state->gpu_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        true, nullptr
    );

    if (!state->gpu_device) {
        SDL_Log("GPU Create Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create Window
    state->window = SDL_CreateWindow("Cow Game Engine [Hyper2D]", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (!state->window || !SDL_ClaimWindowForGPUDevice(state->gpu_device, state->window)) {
        return SDL_APP_FAILURE;
    }

    // --- SCENE INITIALIZATION ---
    state->last_tick = SDL_GetTicks();

    // Create the generic Root
    state->root_node = new Node("RootScene");

    // TEST: Create a Player Character
    SpriteNode* player = new SpriteNode("Hero");
    player->position_x = 0.0f;
    player->position_y = 0.0f;
    player->position_z = 0.0f;
    player->billboard_mode = BillboardMode::Y_AXIS; // Face the camera!

    state->root_node->AddChild(player);

    return SDL_APP_CONTINUE;
}

// 2. App Event
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    EngineState* state = (EngineState*)appstate;
    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

// 3. App Iterate
SDL_AppResult SDL_AppIterate(void* appstate) {
    EngineState* state = (EngineState*)appstate;

    // A. Time Calculation
    uint64_t current_tick = SDL_GetTicks();
    state->delta_time = (double)(current_tick - state->last_tick) / 1000.0;
    state->last_tick = current_tick;

    // B. Scene Update (Logic)
    if (state->root_node) {
        state->root_node->Process(state->delta_time);
    }

    // C. Rendering
    SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(state->gpu_device);
    if (!cmd_buf) return SDL_APP_FAILURE;

    SDL_GPUTexture* swapchain_tex;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buf, state->window, &swapchain_tex, nullptr, nullptr)) {
        SDL_CancelGPUCommandBuffer(cmd_buf);
        return SDL_APP_FAILURE;
    }

    SDL_GPUColorTargetInfo color_target = {};
    color_target.texture = swapchain_tex;
    color_target.clear_color = { 0.95f, 0.95f, 0.9f, 1.0f }; // Cow White
    color_target.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd_buf, &color_target, 1, nullptr);

    // --- SCENE DRAW ---
    if (state->root_node) {
        state->root_node->Render(pass);
    }

    SDL_EndGPURenderPass(pass);
    SDL_SubmitGPUCommandBuffer(cmd_buf);

    return SDL_APP_CONTINUE;
}

// 4. App Quit
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    EngineState* state = (EngineState*)appstate;
    if (state) {
        delete state->root_node; // Clean up the entire tree
        SDL_ReleaseWindowFromGPUDevice(state->gpu_device, state->window);
        SDL_DestroyWindow(state->window);
        SDL_DestroyGPUDevice(state->gpu_device);
        delete state;
    }
}