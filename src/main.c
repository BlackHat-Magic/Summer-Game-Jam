#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>

// hold global resources
typedef struct {
    SDL_Window* window;
    SDL_GPUDevice* device;
    SDL_GPUGraphicsPipeline* pipeline;
} AppContext;

// helper: load binary file
static Uint8* load_file (const char* path, size_t* size) {
    SDL_IOStream* file = SDL_IOFromFile(path, "rb");
    if (!file) return NULL;

    Sint64 file_size = SDL_GetIOSize (file);
    Uint8* buffer = (Uint8*) malloc (file_size);
    SDL_ReadIO (file, buffer, file_size);
    SDL_CloseIO (file);
    *size = file_size;

    return buffer;
}

SDL_AppResult SDL_AppInit (void** appstate, int argc, char** argv) {
    // Initialize SDL
    if (!SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log ("Failed to init SDL: %s", SDL_GetError ());
        return SDL_APP_FAILURE;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow ("Space Pirates", 640, 480, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log ("Failed to create window: %s", SDL_GetError ());
    }

    // initialize GPU Device
    SDL_GPUShaderFormat formats = SDL_GPU_SHADERFORMAT_SPIRV;
    SDL_GPUDevice* device = SDL_CreateGPUDevice (formats, false, NULL);
    if (!device) {
        SDL_Log ("Failed to create GPU Device: %s", SDL_GetError ());
        return SDL_APP_FAILURE;
    }
    SDL_Log ("Using GPU Driver: %s", SDL_GetGPUDeviceDriver (device));

    // claim window
    if (!SDL_ClaimWindowForGPUDevice (device, window)) {
        SDL_Log ("Failed to claim window: %s", SDL_GetError ());
        return SDL_APP_FAILURE;
    }

    // load shaders
    size_t vert_size, frag_size;
    Uint8* vert_code = load_file ("./shaders/triangle.vert.spv", &vert_size);
    Uint8* frag_code = load_file ("./shaders/triangle.frag.spv", &frag_size);
    if (!vert_code || !frag_code) {
        SDL_Log ("Failed to load shaders");
        return SDL_APP_FAILURE;
    }

    // create shaders
    SDL_GPUShaderCreateInfo vert_info = {
        .code = vert_code,
        .code_size = vert_size,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_samplers = 0,
        .num_storage_buffers = 0,
        .num_storage_textures = 0,
        .num_uniform_buffers = 0
    };
    SDL_GPUShader* vert_shader = SDL_CreateGPUShader (device, &vert_info);
    free (vert_code);
    SDL_GPUShaderCreateInfo frag_info = {
        .code = frag_code,
        .code_size = frag_size,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
        .num_samplers = 0,
        .num_storage_buffers = 0,
        .num_storage_textures = 0,
        .num_uniform_buffers = 0,
    };
    SDL_GPUShader* frag_shader = SDL_CreateGPUShader (device, &frag_info);
    free (frag_code);
    if (!vert_shader || !frag_shader) {
        SDL_Log ("Failed to create shaders: %s", SDL_GetError ());
        return SDL_APP_FAILURE;
    }

    // create graphics pipeline
    SDL_GPUColorTargetDescription color_desc = {
        .format = SDL_GetGPUSwapchainTextureFormat (device, window)
    };
    SDL_GPUGraphicsPipelineCreateInfo pipeline_info = {
        .target_info = {
            .num_color_targets = 1,
            .color_target_descriptions = &color_desc,
        },
        .vertex_shader = vert_shader,
        .fragment_shader = frag_shader,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .multisample_state = {
            .sample_count = SDL_GPU_SAMPLECOUNT_1,
        }
    };
    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipeline_info);
    // release shaders (pipeline holds reference)
    SDL_ReleaseGPUShader (device, vert_shader);
    SDL_ReleaseGPUShader (device, frag_shader);
    if (!pipeline) {
        SDL_Log ("Failed to create pipeline: %s", SDL_GetError ());
        return SDL_APP_FAILURE;
    }

    // store in app state
    AppContext* context = (AppContext*) malloc (sizeof (AppContext));
    context->window = window;
    context->device = device;
    context->pipeline = pipeline;
    *appstate = context;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent (void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// runs every frame
SDL_AppResult SDL_AppIterate (void* appstate) {
    AppContext* context = (AppContext*) appstate;

    // Acquire command buffer
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer (context->device);
    if (!cmd) {
        SDL_Log ("Failed to acquire command buffer: %s", SDL_GetError ());
        return SDL_APP_FAILURE;
    }

    // Acquire swapchain texture (window render target)
    SDL_GPUTexture* swapchain;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd, context->window, &swapchain, NULL, NULL)) {
        SDL_Log ("Failed to acquire swapchain: %s", SDL_GetError ());
        SDL_SubmitGPUCommandBuffer (cmd);
        return SDL_APP_FAILURE;
    }

    if (swapchain) {
        // clear to black
        SDL_GPUColorTargetInfo target_info = {
            .texture = swapchain,
            .clear_color = {0.0f, 0.0f, 0.0f, 1.0f},
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
            .cycle = true,
        };

        // begin render pass
        SDL_GPURenderPass* pass = SDL_BeginGPURenderPass (cmd, &target_info, 1, NULL);

        // bind gpu pipeline and draw triangle
        // 3 vertices; no vertex buffer needed since vertex shader generates them
        SDL_BindGPUGraphicsPipeline (pass, context->pipeline);
        SDL_DrawGPUPrimitives (pass, 3, 1, 0, 0);

        SDL_EndGPURenderPass(pass);
    }

    SDL_SubmitGPUCommandBuffer (cmd);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit (void* appstate, SDL_AppResult result) {
    AppContext* context = (AppContext*) appstate;
    if (context) {
        SDL_ReleaseGPUGraphicsPipeline (context->device, context->pipeline);
        SDL_ReleaseWindowFromGPUDevice (context->device, context->window);
        SDL_DestroyGPUDevice (context->device);
        SDL_DestroyWindow (context->window);
        free (context);
    }
    SDL_Quit ();
}