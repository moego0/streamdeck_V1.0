#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "lvgl/lvgl.h"
#include "simulator_input.h"
#include "debug_panel.h"
#include "ui_events.h"

// Display dimensions
#define SIM_HOR_RES 172
#define SIM_VER_RES 320
#define SIM_SCALE 2  // 2x scaling for visibility

// Global simulator state
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *disp_buf_texture = NULL;

// Device state
typedef struct {
    uint8_t current_profile;
    bool mic_muted;
    uint8_t encoder1_mode;  // 0=Volume, 1=Mic, 2=Timeline, 3=Zoom, 4=Scroll
    uint8_t encoder2_mode;
    uint8_t key_labels[10];
    uint32_t key_colors[10];
    uint8_t volume;
    uint8_t mic_level;
    uint8_t brightness;
} device_state_t;

static device_state_t dev_state = {
    .current_profile = 1,
    .mic_muted = false,
    .encoder1_mode = 0,  // Volume
    .encoder2_mode = 1,  // Mic Sensitivity
    .volume = 70,
    .mic_level = 45,
    .brightness = 100,
};

// Debug event log
#define DEBUG_LOG_SIZE 32
static char debug_log[DEBUG_LOG_SIZE][64];
static uint32_t debug_log_idx = 0;

/**
 * Initialize SDL and create window
 */
static bool init_sdl(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Stream Deck UI Simulator - 172x320 Portrait",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SIM_HOR_RES * SIM_SCALE,
        SIM_VER_RES * SIM_SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Set render scale for 2x pixel size
    SDL_RenderSetScale(renderer, SIM_SCALE, SIM_SCALE);

    // Create display buffer texture
    disp_buf_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SIM_HOR_RES,
        SIM_VER_RES
    );

    if (!disp_buf_texture) {
        fprintf(stderr, "Failed to create display texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

/**
 * Log debug event to circular buffer
 */
void debug_log_event(const char *format, ...)
{
    char buffer[64];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    strncpy(debug_log[debug_log_idx], buffer, 63);
    debug_log_idx = (debug_log_idx + 1) % DEBUG_LOG_SIZE;
}

/**
 * Render debug panel (bottom left corner, overlay)
 */
static void render_debug_panel(void)
{
    SDL_SetRenderDrawColor(renderer, 26, 31, 58, 200);  // Dark background
    SDL_Rect debug_rect = {4, 280, 80, 35};
    SDL_RenderFillRect(renderer, &debug_rect);

    SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255);  // Blue border
    SDL_RenderDrawRect(renderer, &debug_rect);

    // Display last 3 events
    int log_start = (debug_log_idx + DEBUG_LOG_SIZE - 3) % DEBUG_LOG_SIZE;
    for (int i = 0; i < 3; i++) {
        int idx = (log_start + i) % DEBUG_LOG_SIZE;
        // Note: Proper text rendering would use SDL_ttf or LVGL's font renderer
        printf("[DEBUG] %s\n", debug_log[idx]);
    }
}

/**
 * Handle keyboard input and map to device events
 */
static void handle_keyboard_input(SDL_KeyboardEvent *key_event)
{
    if (key_event->type != SDL_KEYDOWN) return;

    SDL_Keysym keysym = key_event->keysym;

    // Map keys:
    // 1-9, 0: macro keys 0-9
    // Arrow Up/Down: encoder1 rotation
    // Arrow Left/Right: encoder2 rotation
    // Return/Space: encoder press
    // M: mic mute toggle
    // [, ]: profile prev/next

    if (keysym.sym >= SDLK_1 && keysym.sym <= SDLK_9) {
        uint8_t key_idx = keysym.sym - SDLK_1;
        debug_log_event("Macro Key %d pressed", key_idx);
        ui_event_macro_key(key_idx);
    }
    else if (keysym.sym == SDLK_0) {
        debug_log_event("Macro Key 9 pressed");
        ui_event_macro_key(9);
    }
    else if (keysym.sym == SDLK_UP) {
        debug_log_event("ENC1 CW");
        ui_event_encoder_rotate(0, 1);  // Encoder 0, clockwise
    }
    else if (keysym.sym == SDLK_DOWN) {
        debug_log_event("ENC1 CCW");
        ui_event_encoder_rotate(0, -1);  // Encoder 0, counter-clockwise
    }
    else if (keysym.sym == SDLK_LEFT) {
        debug_log_event("ENC2 CW");
        ui_event_encoder_rotate(1, 1);  // Encoder 1, clockwise
    }
    else if (keysym.sym == SDLK_RIGHT) {
        debug_log_event("ENC2 CCW");
        ui_event_encoder_rotate(1, -1);  // Encoder 1, counter-clockwise
    }
    else if (keysym.sym == SDLK_RETURN || keysym.sym == SDLK_SPACE) {
        debug_log_event("ENC1 Press");
        ui_event_encoder_press(0);
    }
    else if (keysym.sym == SDLK_m) {
        dev_state.mic_muted = !dev_state.mic_muted;
        debug_log_event("Mic: %s", dev_state.mic_muted ? "MUTED" : "ON");
        ui_event_mic_toggle();
    }
    else if (keysym.sym == SDLK_LEFTBRACKET) {
        if (dev_state.current_profile > 1) {
            dev_state.current_profile--;
            debug_log_event("Prof: %d", dev_state.current_profile);
            ui_event_profile_changed(dev_state.current_profile);
        }
    }
    else if (keysym.sym == SDLK_RIGHTBRACKET) {
        if (dev_state.current_profile < 8) {
            dev_state.current_profile++;
            debug_log_event("Prof: %d", dev_state.current_profile);
            ui_event_profile_changed(dev_state.current_profile);
        }
    }
}

/**
 * Main event loop
 */
static bool handle_events(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;

            case SDL_KEYDOWN:
                handle_keyboard_input(&event.key);
                break;

            default:
                break;
        }
    }

    return true;
}

/**
 * LVGL flush callback - copy display buffer to SDL texture
 */
static void lv_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                        lv_color_t *color_p)
{
    // In a real implementation, this would update the SDL texture with the
    // color data from LVGL's display buffer.
    // For now, we'll just indicate that the flush is complete.

    lv_disp_flush_ready(disp_drv);
}

/**
 * Render frame to screen
 */
static void render_frame(void)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 10, 14, 39, 255);
    SDL_RenderClear(renderer);

    // Render LVGL display
    lv_task_handler();

    // Render debug panel
    render_debug_panel();

    // Present
    SDL_RenderPresent(renderer);
}

/**
 * Main entry point
 */
int main(int argc, char *argv[])
{
    printf("Stream Deck LVGL Simulator Starting...\n");
    printf("Display: 172x320 portrait\n");
    printf("Scaling: 2x (for visibility)\n\n");

    printf("Input Mapping:\n");
    printf("  Macro Keys:   1-9 (for keys 1-9), 0 (for key 10)\n");
    printf("  Encoder 1:    UP/DOWN rotate, ENTER/SPACE press\n");
    printf("  Encoder 2:    LEFT/RIGHT rotate\n");
    printf("  Mic Toggle:   M\n");
    printf("  Profile Prev: [\n");
    printf("  Profile Next: ]\n");
    printf("  Quit:         CLOSE window\n\n");

    if (!init_sdl()) {
        fprintf(stderr, "Failed to initialize SDL\n");
        return 1;
    }

    // Initialize LVGL
    lv_init();

    // Create display driver and buffer
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[SIM_HOR_RES * 10];  // Partial buffer
    lv_disp_buf_init(&disp_buf, buf, NULL, SIM_HOR_RES * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = lv_flush_cb;
    disp_drv.hor_res = SIM_HOR_RES;
    disp_drv.ver_res = SIM_VER_RES;
    lv_disp_drv_register(&disp_drv);

    // Initialize UI screens
    ui_init(&dev_state);

    // Load home screen
    ui_show_home();

    debug_log_event("Simulator Started");
    debug_log_event("Profile: %d", dev_state.current_profile);
    debug_log_event("Ready");

    // Main loop
    bool running = true;
    uint32_t last_time = SDL_GetTicks();

    while (running) {
        uint32_t current_time = SDL_GetTicks();
        uint32_t elapsed = current_time - last_time;
        last_time = current_time;

        running = handle_events();
        lv_tick_inc(elapsed);
        render_frame();

        // Cap at ~60 FPS
        SDL_Delay(16);
    }

    printf("\nSimulator Shutdown\n");

    // Cleanup
    SDL_DestroyTexture(disp_buf_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
