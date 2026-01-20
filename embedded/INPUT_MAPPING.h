/**
 * Input Mapping & Device Event System
 * 
 * This file documents the complete input mapping for the Stream Deck device.
 * It provides the reference implementation for button/encoder to UI event translation.
 */

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// INPUT DEVICE DEFINITIONS
// ============================================================================

#define NUM_MACRO_KEYS 10
#define NUM_ENCODERS 2
#define NUM_TACT_BUTTONS 3

// Macro Key Layout (2x5 grid)
// [0] [1] [2] [3] [4]
// [5] [6] [7] [8] [9]

// Tact Buttons
#define TACT_BTN_1_MIC_MUTE 0
#define TACT_BTN_2_PROFILE_PREV 1
#define TACT_BTN_3_PROFILE_NEXT 2

// ============================================================================
// INPUT EVENT MAPPING
// ============================================================================

/**
 * INPUT MAPPING TABLE
 * 
 * Physical Input           | Event                | Context
 * ========================================================================================
 * 
 * MACRO KEYS (0-9):
 *   Key 0-9 pressed        | ui_event_macro_key(n)          | Any screen
 *   Key X pressed          | Highlight key X on Macros      | Macros screen
 *                          | Send macro to PC via USB       | All screens
 * 
 * ENCODER 1 (CW/CCW + Push):
 *   Rotation CW            | ui_event_encoder_rotate(0, +1) | Context-dependent:
 *                          |   - Home: Adjust volume
 *                          |   - Profiles: Next profile
 *                          |   - Encoders: Scroll down
 *   Rotation CCW           | ui_event_encoder_rotate(0, -1) | 
 *   Push                   | ui_event_encoder_press(0)      | Context-dependent:
 *                          |   - Home: Open menu/profiles
 *                          |   - Profiles: Select profile
 *                          |   - Encoders: Apply encoder mode
 * 
 * ENCODER 2 (CW/CCW + Push):
 *   Rotation CW            | ui_event_encoder_rotate(1, +1) | Context-dependent:
 *                          |   - Home: Adjust secondary value
 *                          |   - Lists: Scroll within list
 *   Rotation CCW           | ui_event_encoder_rotate(1, -1) |
 *   Push                   | ui_event_encoder_press(1)      | Context-dependent
 * 
 * TACT BUTTONS (3x momentary):
 *   Button 1 (Mic mute)    | ui_event_mic_toggle()          | All screens (global)
 *                          | Update UI mic indicator
 *                          | Send mute state to PC
 *   Button 2 (Profile<)    | ui_event_profile_prev()        | Context-dependent:
 *                          |   - Home: Decrease profile
 *                          |   - Other: Go back to home
 *   Button 3 (Profile>)    | ui_event_profile_next()        | Context-dependent:
 *                          |   - Home: Increase profile
 *                          |   - Other: Go forward/open menu
 * 
 * ========================================================================================
 */

// ============================================================================
// KEYBOARD SIMULATOR MAPPING (for PC Simulator)
// ============================================================================

/**
 * KEYBOARD -> DEVICE INPUT MAPPING (for simulator testing)
 * 
 * Keyboard Key   | Device Input           | Notes
 * ===============================================================
 * 1-9            | Macro Key 1-9          |
 * 0              | Macro Key 10 (Key 9)   |
 * UP             | Encoder 1 CW (rotate)  |
 * DOWN           | Encoder 1 CCW          |
 * LEFT           | Encoder 2 CW           |
 * RIGHT          | Encoder 2 CCW          |
 * RETURN/SPACE   | Encoder 1 Push         |
 * M              | Tact Button 1 (Mic)    |
 * [              | Tact Button 2 (Prev)   |
 * ]              | Tact Button 3 (Next)   |
 * ESC            | Close simulator        |
 * ===============================================================
 */

// ============================================================================
// SCREEN NAVIGATION FLOW
// ============================================================================

/**
 * NAVIGATION GRAPH
 * 
 *     [HOME SCREEN]
 *          |
 *    Press Enc1 / ] button
 *          |
 *          v
 *    [PROFILES SCREEN]
 *      Rotate Enc1 to select
 *      Press Enc1 to confirm
 *          |
 *    Press [ button (back)
 *          |
 *          v returns to HOME
 * 
 * 
 *     [HOME SCREEN]
 *          |
 *   Right menu button or long press
 *          |
 *          v
 *    [MENU SCREEN]
 *       (Optional)
 *       - Macros
 *       - Encoders
 *       - RGB/Theme
 *       - Exit
 * 
 *    [MACROS SCREEN]
 *    Shows 10 key grid
 *    Highlight on key press
 *    Press [ back
 * 
 *    [ENCODERS SCREEN]
 *    Encoder mode configuration
 *    Press Enc1/Enc2 to apply
 *    Press [ to go back
 * 
 *    [RGB SCREEN]
 *    Theme selection
 *    Accent color picker
 *    Brightness slider
 *    Press ] to apply
 *    Press [ to cancel
 * 
 * QUICK OVERLAY (global, auto-dismiss 1-2s):
 *    - Profile change
 *    - Mic mute toggle
 *    - Encoder mode change
 */

// ============================================================================
// PC COMMUNICATION PROTOCOL (USB HID)
// ============================================================================

/**
 * USB HID REPORT FORMAT
 * 
 * The device sends HID reports to the PC for macro execution.
 * 
 * Report Structure (64 bytes):
 *   [0] - Report ID (0x01 for macro event)
 *   [1] - Event type:
 *       0x00 = Macro key press
 *       0x01 = Encoder rotate
 *       0x02 = Status update
 *   [2-3] - Payload (context-dependent)
 *   [4-63] - Reserved
 * 
 * Macro Key Report:
 *   [1] = 0x00 (type)
 *   [2] = key_index (0-9)
 *   [3] = 0x01 (press) or 0x00 (release)
 * 
 * Status Report:
 *   [1] = 0x02 (type)
 *   [2] = current_profile
 *   [3] = mic_muted (bool)
 *   [4] = volume (0-100)
 *   [5] = mic_level (0-100)
 * 
 * PC App Configuration:
 *   Receives via USB COM port (CDC) or HID report descriptor
 *   Sends profile/key configurations back
 */

// ============================================================================
// DEVICE STATE UPDATES
// ============================================================================

/**
 * DEVICE STATE STRUCTURE
 */
typedef struct {
    uint8_t current_profile;         // 1-8
    bool mic_muted;                  // True/False
    uint8_t encoder1_mode;           // 0=Volume, 1=Mic, 2=Timeline, 3=Zoom, 4=Scroll
    uint8_t encoder2_mode;           // Same as above
    
    // Per-profile configuration
    uint8_t key_macros[10];          // Which macro for each key in current profile
    uint32_t key_colors[10];         // RGB color for each key LED
    char key_labels[10][16];         // Display label for each key
    
    // Display values
    uint8_t volume;                  // 0-100
    uint8_t mic_level;               // 0-100 (input level)
    uint8_t brightness;              // 0-100 (LED brightness limit)
    
    // Theme
    uint8_t theme;                   // 0=Dark, 1=Light
    uint8_t accent_color;            // 0=Blue, 1=Cyan, 2=Green, 3=Orange, 4=Red
} device_state_t;

// ============================================================================
// ACCEPTANCE CHECKLIST
// ============================================================================

/**
 * VERIFICATION CHECKLIST
 * 
 * ✓ Profile number visible on every screen
 *   - Home: Top-right badge "PROF X"
 *   - Profiles: Top-right badge
 *   - Macros: Top-right badge
 *   - Encoders: Top-right badge
 *   - RGB: Top-right badge
 * 
 * ✓ Key press highlights correct slot
 *   - Macro Grid screen shows 10 slots (2x5 layout)
 *   - Physical key 0-9 press highlights corresponding slot
 *   - "Last pressed" indicator updated
 *   - Auto-reset highlight after 500ms
 * 
 * ✓ Profile switching works
 *   - [ button: previous profile
 *   - ] button: next profile
 *   - Profile selector screen with encoder navigation
 *   - Profile number persisted across device resets
 * 
 * ✓ Encoder assignment screen functional
 *   - Select mode from list for each encoder
 *   - Press encoder button to apply
 *   - Display current mode for each encoder
 *   - Mode persisted per-profile
 * 
 * ✓ Simulator runs and shows 172x320 portrait
 *   - Keyboard input mapping functional
 *   - Debug panel shows last 3 events
 *   - ~60 FPS rendering
 *   - SDL window title shows resolution and scaling (2x)
 */

#endif // INPUT_MAPPING_H
