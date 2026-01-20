#ifndef SIMULATOR_INPUT_H
#define SIMULATOR_INPUT_H

#include <stdint.h>
#include <stdbool.h>

// Input event callbacks
void ui_event_macro_key(uint8_t key_index);
void ui_event_encoder_rotate(uint8_t encoder_id, int8_t direction);
void ui_event_encoder_press(uint8_t encoder_id);
void ui_event_mic_toggle(void);
void ui_event_profile_changed(uint8_t new_profile);

// Device state logging
void debug_log_event(const char *format, ...);

#endif // SIMULATOR_INPUT_H
