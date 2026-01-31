#define SDL_MAIN_USE_CALLBACKS 1  // Use the new "Main Callbacks" architecture (Cleaner for engines)
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

// Engine State
struct EngineState {
    SDL_Window* window = nullptr;
    SDL_GPUDevice* gpu_device = nullptr;
    bool quit = false;
};

// 1. App Init: Runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    SDL_SetAppMetadata("Cow Game Engine", "1.0", "com.cowengine.game");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("SDL Init Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create the new App State
    EngineState* state = new EngineState();
    *appstate = state;

    // Create the GPU Device (Auto-selects Vulkan/D3D12/Metal)
    // We explicitly ask for a shader format (SPIR-V is good for cross-platform)
    state->gpu_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        true, // Debug mode (helpful for dev)
        nullptr // default driver
    );

    if (!state->gpu_device) {
        SDL_Log("GPU Create Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create Window (Must be claimable by GPU)
    state->window = SDL_CreateWindow(
        "Cow Game Engine [Hyper2D]",
        1280, 720,
        SDL_WINDOW_RESIZABLE
    );

    if (!state->window) {
        SDL_Log("Window Create Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Link the Window to the GPU
    if (!SDL_ClaimWindowForGPUDevice(state->gpu_device, state->window)) {
        SDL_Log("GPU Claim Window Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

// 2. App Event: Handles input (Keys, Mouse)
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    EngineState* state = (EngineState*)appstate;

    if (event->type == SDL_EVENT_QUIT) {
        state->quit = true;
        return SDL_APP_SUCCESS;
    }

    // Quick exit with Escape key
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

// 3. App Iterate: Runs every frame (Update + Render)
SDL_AppResult SDL_AppIterate(void* appstate) {
    EngineState* state = (EngineState*)appstate;

    // A. Acquire Command Buffer
    SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(state->gpu_device);
    if (!cmd_buf) return SDL_APP_FAILURE; // GPU busy or failed

    // B. Acquire Texture (The Backbuffer)
    SDL_GPUTexture* swapchain_tex;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buf, state->window, &swapchain_tex, nullptr, nullptr)) {
        SDL_CancelGPUCommandBuffer(cmd_buf);
        return SDL_APP_FAILURE;
    }

    // C. Setup Rendering (Clear Screen to "Cow White")
    SDL_GPUColorTargetInfo color_target_info = {};
    color_target_info.texture = swapchain_tex;
    color_target_info.clear_color = { 0.95f, 0.95f, 0.9f, 1.0f }; // Creamy white
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, nullptr);

    // -- Draw commands would go here --

    SDL_EndGPURenderPass(render_pass);

    // D. Submit to GPU
    SDL_SubmitGPUCommandBuffer(cmd_buf);

    return SDL_APP_CONTINUE;
}

// 4. App Quit: Cleanup
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    EngineState* state = (EngineState*)appstate;
    if (state) {
        SDL_ReleaseWindowFromGPUDevice(state->gpu_device, state->window);
        SDL_DestroyWindow(state->window);
        SDL_DestroyGPUDevice(state->gpu_device);
        delete state;
    }
}