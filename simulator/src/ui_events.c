#include "ui_events.h"
#include "lvgl/lvgl.h"
#include <stdio.h>

static device_state_t *g_dev_state = NULL;

// Screen references
static lv_obj_t *scr_home = NULL;
static lv_obj_t *scr_profiles = NULL;
static lv_obj_t *scr_macros = NULL;
static lv_obj_t *scr_encoders = NULL;
static lv_obj_t *scr_rgb = NULL;
static lv_obj_t *scr_overlay = NULL;

static lv_obj_t *current_screen = NULL;

/**
 * Initialize UI system with device state reference
 */
void ui_init(device_state_t *dev_state)
{
    g_dev_state = dev_state;

    // Create screens (placeholder - in real implementation, load from XML)
    scr_home = lv_obj_create(NULL);
    scr_profiles = lv_obj_create(NULL);
    scr_macros = lv_obj_create(NULL);
    scr_encoders = lv_obj_create(NULL);
    scr_rgb = lv_obj_create(NULL);
    scr_overlay = lv_obj_create(NULL);

    printf("[UI] Initialized with device state\n");
}

/**
 * Show home screen
 */
void ui_show_home(void)
{
    if (scr_home && current_screen != scr_home) {
        lv_scr_load(scr_home);
        current_screen = scr_home;
        printf("[UI] Loaded: Home Screen\n");
    }
}

/**
 * Show profiles screen
 */
void ui_show_profiles(void)
{
    if (scr_profiles && current_screen != scr_profiles) {
        lv_scr_load(scr_profiles);
        current_screen = scr_profiles;
        printf("[UI] Loaded: Profiles Screen\n");
    }
}

/**
 * Show macros screen
 */
void ui_show_macros(void)
{
    if (scr_macros && current_screen != scr_macros) {
        lv_scr_load(scr_macros);
        current_screen = scr_macros;
        printf("[UI] Loaded: Macros Screen\n");
    }
}

/**
 * Show encoders screen
 */
void ui_show_encoders(void)
{
    if (scr_encoders && current_screen != scr_encoders) {
        lv_scr_load(scr_encoders);
        current_screen = scr_encoders;
        printf("[UI] Loaded: Encoders Screen\n");
    }
}

/**
 * Show RGB/theme screen
 */
void ui_show_rgb(void)
{
    if (scr_rgb && current_screen != scr_rgb) {
        lv_scr_load(scr_rgb);
        current_screen = scr_rgb;
        printf("[UI] Loaded: RGB Settings Screen\n");
    }
}

/**
 * Show quick overlay notification
 */
void ui_show_quick_overlay(const char *message)
{
    printf("[UI] Overlay: %s\n", message);
    // In real implementation, would create and auto-dismiss overlay
}

/**
 * Handle macro key press
 */
void ui_event_macro_key(uint8_t key_index)
{
    if (key_index < 10) {
        printf("[EVENT] Macro Key %d pressed\n", key_index);

        // If on macros screen, highlight the key
        if (current_screen == scr_macros) {
            // Trigger visual feedback on key slot
        }
    }
}

/**
 * Handle encoder rotation
 */
void ui_event_encoder_rotate(uint8_t encoder_id, int8_t direction)
{
    const char *dir_str = (direction > 0) ? "CW" : "CCW";
    printf("[EVENT] Encoder %d rotated %s\n", encoder_id, dir_str);

    // Adjust values based on current screen and encoder mode
    if (current_screen == scr_profiles) {
        // Scroll profile list
    }
    else if (current_screen == scr_home) {
        // Adjust volume or other value
    }
}

/**
 * Handle encoder press/button
 */
void ui_event_encoder_press(uint8_t encoder_id)
{
    printf("[EVENT] Encoder %d pressed\n", encoder_id);

    // Navigate to next screen or select item
    if (current_screen == scr_home) {
        ui_show_profiles();
    }
    else if (current_screen == scr_profiles) {
        printf("[EVENT] Profile %d selected\n", g_dev_state->current_profile);
        ui_show_home();
    }
}

/**
 * Handle mic mute toggle
 */
void ui_event_mic_toggle(void)
{
    printf("[EVENT] Mic toggled: %s\n", g_dev_state->mic_muted ? "MUTED" : "ON");
    ui_show_quick_overlay(g_dev_state->mic_muted ? "Mic: MUTED" : "Mic: ON");
}

/**
 * Handle profile change
 */
void ui_event_profile_changed(uint8_t new_profile)
{
    printf("[EVENT] Profile changed to: %d\n", new_profile);
    ui_show_quick_overlay("Profile switched");
}
