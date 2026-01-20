#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#include <stdint.h>

// Forward declaration
typedef struct {
    uint8_t current_profile;
    bool mic_muted;
    uint8_t encoder1_mode;
    uint8_t encoder2_mode;
    uint8_t key_labels[10];
    uint32_t key_colors[10];
    uint8_t volume;
    uint8_t mic_level;
    uint8_t brightness;
} device_state_t;

// UI initialization
void ui_init(device_state_t *dev_state);

// Screen management
void ui_show_home(void);
void ui_show_profiles(void);
void ui_show_macros(void);
void ui_show_encoders(void);
void ui_show_rgb(void);
void ui_show_quick_overlay(const char *message);

// Event handlers
void ui_event_macro_key(uint8_t key_index);
void ui_event_encoder_rotate(uint8_t encoder_id, int8_t direction);
void ui_event_encoder_press(uint8_t encoder_id);
void ui_event_mic_toggle(void);
void ui_event_profile_changed(uint8_t new_profile);

#endif // UI_EVENTS_H
