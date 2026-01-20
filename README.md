# Stream Deck LVGL Pro UI - Complete Documentation

**Project**: Stream Deck Macro Pad UI  
**Platform**: STM32F103C8T6 + 1.47" ST7789 TFT (172×320 portrait)  
**UI Framework**: LVGL Pro (XML workflow)  
**Date**: January 2026

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Directory Structure](#directory-structure)
3. [Screen Descriptions](#screen-descriptions)
4. [Component Reference](#component-reference)
5. [Style/Theme System](#styletheme-system)
6. [PC Simulator Setup](#pc-simulator-setup)
7. [Embedded Integration](#embedded-integration)
8. [Input Mapping Reference](#input-mapping-reference)
9. [Build Instructions](#build-instructions)
10. [Acceptance Checklist](#acceptance-checklist)

---

## Project Overview

This is a complete LVGL Pro XML project for a **Stream Deck / Macro Pad device** with:

- **Display**: 1.47" TFT ST7789, 172×320 pixels, portrait orientation
- **Inputs**: 10 macro keys (2×5 grid), 2 rotary encoders (with push), 3 tact buttons
- **Outputs**: Per-key RGB LEDs, USB HID to PC
- **Features**:
  - 8 user profiles (selectable from device)
  - Per-key macro/shortcut configuration
  - Encoder mode assignment (Volume, Mic Sensitivity, Timeline, Zoom, Scroll)
  - RGB theme selection (Dark/Light + accent colors)
  - Quick overlay notifications
  - High-contrast, minimal-animation UI optimized for STM32F103 performance

### Hardware Layout

```
┌─────────────────────────────────────────────────────┐
│  Display Area (172×320)                             │
│                                                     │
│  +── Profile Badge (top right)                     │
│  |   Status Bar (USB, Mic, Encoder modes)          │
│  |                                                 │
│  |  ┌─ Home Screen / Dashboard                    │
│  |  │  - Profile #1                               │
│  |  │  - Connection status                        │
│  |  │  - Mic status                               │
│  |  │  - Volume, Mic Level, Brightness tiles      │
│  |  │  - Active app name                          │
│  |  │                                             │
│  |  ├─ Profile Selector Screen                   │
│  |  │  - List profiles 1-8                        │
│  |  │  - Rotate Enc1 to select                    │
│  |  │  - Press Enc1 to confirm                    │
│  |  │                                             │
│  |  ├─ Macro Grid Screen                         │
│  |  │  - 10-key visual grid (2×5)                 │
│  |  │  - Each slot shows label + color indicator  │
│  |  │  - Highlights on key press                  │
│  |  │                                             │
│  |  ├─ Encoder Assignment Screen                 │
│  |  │  - Mode select for Encoder 1                │
│  |  │  - Mode select for Encoder 2                │
│  |  │  - Press encoder to apply                   │
│  |  │                                             │
│  |  ├─ RGB / Theme Screen                        │
│  |  │  - Theme toggle (Dark/Light)                │
│  |  │  - Accent color picker (5 colors)           │
│  |  │  - LED brightness slider                    │
│  |  │                                             │
│  |  └─ Quick Overlay (global)                    │
│  |     - Auto-dismiss notifications               │
│  |     - Profile changes, Mic toggle, etc.        │
│  |                                                 │
│  └─ Navigation via buttons and encoders            │
│                                                     │
└─────────────────────────────────────────────────────┘
```

---

## Directory Structure

```
lvgl_pro/
├── ui/
│   ├── screens/
│   │   ├── scr_home.xml              # Home dashboard
│   │   ├── scr_profiles.xml          # Profile selector
│   │   ├── scr_macros.xml            # Macro grid (10-key)
│   │   ├── scr_encoders.xml          # Encoder mode config
│   │   ├── scr_rgb.xml               # RGB/theme settings
│   │   └── scr_quick_overlay.xml     # Notification overlay
│   │
│   └── components/
│       ├── cmp_profile_badge.xml     # Persistent profile number badge
│       ├── cmp_status_bar.xml        # USB/Mic/Encoder status bar
│       ├── cmp_tile_widget.xml       # Reusable dashboard tile
│       └── cmp_macro_key_slot.xml    # Macro key visual slot
│
├── styles/
│   └── styles.xml                    # All theme definitions (dark/light, colors)
│
├── assets/
│   └── icons/                        # Simple monochrome icons (optional)
│       └── [icons would go here]
│
├── simulator/
│   ├── CMakeLists.txt                # Build configuration
│   ├── include/
│   │   ├── ui_events.h               # UI event interface
│   │   ├── simulator_input.h         # Input mapping
│   │   └── debug_panel.h             # Debug panel
│   └── src/
│       ├── main.c                    # SDL/LVGL simulator entry
│       ├── ui_events.c               # Event handler stubs
│       ├── debug_panel.c             # Debug logging
│       └── simulator_input.c         # Keyboard input mapping
│
├── embedded/
│   ├── main.c                        # STM32F103 integration skeleton
│   ├── Makefile                      # ARM cross-compile rules
│   └── INPUT_MAPPING.h               # Input reference documentation
│
└── README.md                         # This file
```

---

## Screen Descriptions

### 1. Home Dashboard (`scr_home.xml`)

**Purpose**: Default startup screen showing device status and quick metrics

**Layout** (top to bottom):
- **Header** (32px):
  - Profile Badge (left): Large "PROF X" label with accent color
  - USB Status (center): Green ● if connected, Red ● if disconnected
  - Mic Toggle Button (right): "MIC ON" or "MIC MUTED"

- **Status Bar** (22px):
  - Encoder 1 Mode: "ENC1:VOL", "ENC1:MIC", etc.
  - Encoder 2 Mode: "ENC2:MIC", "ENC2:ZOOM", etc.

- **Tile 1 - Volume**:
  - Title: "Volume"
  - Bar: 0-100%
  - Value: "70%"

- **Tile 2 - Mic Level**:
  - Title: "Mic Level"
  - Bar: 0-100%
  - Value: "45%"

- **Tile 3 - RGB Brightness**:
  - Title: "RGB Brightness"
  - Bar: 0-100%
  - Value: "100%"

- **Footer**:
  - Active App Label: "App: Visual Studio Code"
  - Navigation Hint: "[ ] Prev  Menu  Next [→]"

**Input Behavior**:
- **Encoder 1 (CW/CCW)**: Adjust volume
- **Encoder 1 (Press)**: Open profile selector
- **Tact Button 1 (M)**: Toggle mic mute (global)
- **Tact Button 2 ([)**: Go to previous profile
- **Tact Button 3 (])**: Go to next profile
- **Macro Keys (0-9)**: Execute macros (sent to PC)

---

### 2. Profile Selector (`scr_profiles.xml`)

**Purpose**: Select from 8 available profiles

**Layout**:
- **Header** (32px):
  - Title: "Profiles"
  - Profile Badge: Current profile number

- **Roller/List** (200px):
  - Option items: "Profile 1" ... "Profile 8"
  - Selected item highlighted in accent color with border

- **Controls**:
  - Help text: "Rotate to select"
  - Button: "Press ENC1 to Select"
  - Navigation: "[ ] Back    Next [→]"

**Input Behavior**:
- **Encoder 1 (CW/CCW)**: Scroll list up/down
- **Encoder 1 (Press)**: Select highlighted profile
- **Tact Button 2 ([)**: Cancel / Back to home
- **Tact Button 3 (])**: (Optional) Confirm selection

---

### 3. Macro Grid (`scr_macros.xml`)

**Purpose**: Display 10 macro keys in 2×5 grid with visual feedback

**Layout**:
- **Header** (32px): Title "Macro Keys" + Profile Badge

- **Key Grid** (210px):
  ```
  [F1][F2][F3][F4][F5]
  [F6][F7][F8][F9][F10]
  ```
  Each slot:
  - Button showing key label (F1-F10)
  - Color indicator bar at bottom (per-key RGB color)
  - On press: highlights with accent color + shadow effect
  - Auto-resets highlight after 500ms

- **Footer**:
  - "Last: F1 (Copy)" indicator
  - Navigation: "[ ] Back    Menu [→]"

**Input Behavior**:
- **Macro Keys (0-9)**: Flash/highlight corresponding slot, display action name
- **Tact Button 2 ([)**: Back to home
- **Tact Button 3 (])**: Open menu (optional)

---

### 4. Encoder Assignment (`scr_encoders.xml`)

**Purpose**: Configure each encoder's function

**Layout**:
- **Encoder 1 Section**:
  - Label: "Encoder 1"
  - Mode List (roller):
    - 🔊 Volume
    - 🎤 Mic Sensitivity
    - ⏱️ Timeline Jog
    - 🔍 Zoom
    - 🖱️ Scroll
  - Help: "Press ENC1 to apply"

- **Encoder 2 Section**: (Same layout)

- **Navigation**: "[ ] Back    Next [→]"

**Input Behavior**:
- **Encoder 1 (CW/CCW)**: Scroll Encoder 1 mode list
- **Encoder 1 (Press)**: Apply selected mode for Encoder 1
- **Encoder 2 (CW/CCW)**: Scroll Encoder 2 mode list (if focused)
- **Tact Button 2 ([)**: Back to home
- **Tact Button 3 (])**: Next screen (RGB settings)

---

### 5. RGB / Theme Settings (`scr_rgb.xml`)

**Purpose**: Customize UI theme and LED settings

**Layout**:
- **Theme Section**:
  - Label: "Display Theme"
  - Button Matrix: [Dark] [Light]
  - Currently selected highlighted

- **Accent Color Section**:
  - Label: "Accent Color"
  - Button Matrix (5 columns):
    - ● Blue (default)
    - ● Cyan
    - ● Green
    - ● Orange
    - ● Red
  - Color swatch for each

- **Brightness Section**:
  - Label: "LED Brightness"
  - Slider: 0-100%
  - Value display: "100%"

- **Navigation**: "[ ] Back  Apply [→]"

**Input Behavior**:
- **Encoder 1 (CW/CCW)**: Cycle through theme/color options
- **Encoder 1 (Press)**: Toggle current selection
- **Encoder 2 (CW/CCW)**: Adjust brightness slider
- **Tact Button 2 ([)**: Cancel / Reset to defaults
- **Tact Button 3 (])**: Apply changes

---

### 6. Quick Overlay (`scr_quick_overlay.xml`)

**Purpose**: Temporary notification popup (auto-dismiss 1-2 seconds)

**Layout**:
- **Modal Popup** (centered, ~120×60px):
  - Icon/Emoji (larger font, 28px):
    - 🎯 for profile changes
    - 🎤 for mic toggle
    - ⚙️ for settings
  - Message text below

**Examples**:
- Profile change: "🎯 Profile 2"
- Mic mute: "🎤 Mic: MUTED"
- Encoder mode: "ENC1: Volume"

**Behavior**:
- Auto-triggers on event
- Animates in (optional fade)
- Stays for 1-2 seconds
- Auto-dismisses
- Does not block other input

---

## Component Reference

### `cmp_profile_badge.xml`

A reusable badge showing the current profile number. Appears in top-right of every screen.

**Properties**:
- Size: 50×32 px
- Style Class: `profile_badge_bg`
- Children:
  - Label: Profile number (1-8)

**Usage**: Include in each screen's header

---

### `cmp_status_bar.xml`

Horizontal bar showing system status icons and encoder modes.

**Properties**:
- Size: 172×22 px
- Layout: Flex row with space-between
- Children:
  - USB Status Icon
  - Mic Status Icon
  - Encoder 1 Mode Label
  - Encoder 2 Mode Label

**Usage**: Appears below profile badge on most screens

---

### `cmp_tile_widget.xml`

Reusable dashboard tile for displaying a value with label and progress bar.

**Properties**:
- Size: 160×50 px
- Children:
  - Title label
  - Progress bar
  - Value percentage label

**Usage**: Volume, Mic Level, Brightness on Home screen

**Example markup** (in XML):
```xml
<obj class="lv_obj" id="tile_volume">
  <obj class="lv_label" id="tile_vol_title">
    <text>Volume</text>
  </obj>
  <obj class="lv_bar" id="tile_vol_bar">
    <value>70</value>
  </obj>
  <obj class="lv_label" id="tile_vol_pct">
    <text>70%</text>
  </obj>
</obj>
```

---

### `cmp_macro_key_slot.xml`

Visual representation of a single macro key in the grid.

**Properties**:
- Size: 28×28 px (scales in grid)
- Style: Button with accent border
- Children:
  - Key Label (F1, F2, etc.)
  - Color Indicator (small bar at bottom)

**Pressed State**:
- Background: Accent color (bright)
- Border: Brighter accent
- Shadow: Glow effect

**Usage**: Repeated 10 times in 2×5 grid on Macros screen

---

## Style/Theme System

All styles are defined in [`styles/styles.xml`](styles/styles.xml).

### Color Palette

**Dark Theme** (Default):
- Background: `#0a0e27` (deep blue-black)
- Foreground: `#e8e8e8` (light gray)
- Surface: `#1a1f3a` (lighter blue-black)
- Border: `#2a2f4a` (medium blue-black)

**Light Theme** (Alternative):
- Background: `#f5f5f5` (light gray)
- Foreground: `#1a1a1a` (dark gray/black)
- Surface: `#ffffff` (white)
- Border: `#e0e0e0` (light gray)

**Accent Colors** (User-selectable):
- Blue: `#1e90ff` (default)
- Cyan: `#00bfff`
- Green: `#00ff7f`
- Orange: `#ff8c00`
- Red: `#ff1744`

**Status Colors**:
- USB Connected: `#00ff7f` (green)
- USB Disconnected: `#ff1744` (red)
- Mic Active: `#00ff7f` (green)
- Mic Muted: `#ff8c00` (orange)

### Font Usage

Uses LVGL built-in fonts to minimize flash usage:
- **Montserrat 18**: Profile badge number
- **Montserrat 16**: Screen titles
- **Montserrat 14**: Tile titles
- **Montserrat 13**: Regular labels
- **Montserrat 12**: Status text, buttons
- **Montserrat 11**: Small text, hints
- **Montserrat 10**: Key labels in grid

### Style Classes

Key style classes (referenced in XML):
- `profile_badge_bg`, `profile_number_text`
- `status_bar_bg`, `status_icon`, `status_text`
- `tile_bg`, `tile_title`, `tile_bar`, `tile_value_text`
- `macro_key_slot`, `macro_key_slot:pressed`
- `screen_bg`, `screen_title`, `screen_label`
- `btn_primary`, `btn_primary:pressed`, `btn_secondary`
- `list_item`, `list_item:selected`
- `overlay_bg`, `overlay_text`

---

## PC Simulator Setup

### Prerequisites

1. **SDL2**: Simple DirectMedia Layer library
   ```bash
   # Windows (MSYS2/MinGW)
   pacman -S mingw-w64-x86_64-SDL2
   
   # Linux (Ubuntu/Debian)
   sudo apt-get install libsdl2-dev
   
   # macOS
   brew install sdl2
   ```

2. **LVGL Source**: Download from https://github.com/lvgl/lvgl
   - Extract to `lvgl/` directory in project root

3. **CMake** (optional, or use Makefile):
   ```bash
   cmake --version  # Verify installed
   ```

### Building the Simulator

**Option 1: CMake**
```bash
cd simulator
mkdir build
cd build
cmake ..
make
./streamdeck_simulator
```

**Option 2: Manual GCC**
```bash
cd simulator
gcc -I./include -I../lvgl/src -I/usr/include/SDL2 \
    src/main.c src/ui_events.c src/debug_panel.c \
    `sdl2-config --cflags --libs` \
    -lm -o streamdeck_simulator
./streamdeck_simulator
```

### Running the Simulator

```bash
./streamdeck_simulator
```

**Window**:
- 172×320 pixels (scaled 2x for visibility)
- Title: "Stream Deck UI Simulator - 172x320 Portrait"
- Debug panel (bottom-left corner)

### Keyboard Controls

| Key | Action |
|-----|--------|
| `1`-`9` | Macro keys 1-9 |
| `0` | Macro key 10 |
| **Arrow Up** | Encoder 1 rotate CW |
| **Arrow Down** | Encoder 1 rotate CCW |
| **Arrow Left** | Encoder 2 rotate CW |
| **Arrow Right** | Encoder 2 rotate CCW |
| **Enter** / **Space** | Encoder 1 press |
| **M** | Mic mute toggle (Tact Button 1) |
| **[** | Profile previous (Tact Button 2) |
| **]** | Profile next (Tact Button 3) |
| **Close Window** | Exit simulator |

### Debug Panel

Located at bottom-left of simulator window (overlay).

Displays last 3 events:
- Macro key presses
- Encoder rotations
- Button presses
- Profile/mode changes

Output also goes to console (`stdout`).

---

## Embedded Integration

### Project Structure for STM32

```
streamdeck_firmware/
├── Core/
│   ├── Src/
│   │   ├── main.c                 # Provided skeleton
│   │   ├── stm32f1xx_it.c         # Interrupt handlers
│   │   └── ...
│   └── Inc/
│       └── main.h
│
├── HAL/                            # STM32CubeMX HAL
├── Drivers/
│   ├── display_st7789.c           # ST7789 driver
│   ├── encoders.c                 # Encoder input handling
│   ├── macro_keys.c               # Key matrix scanning
│   └── usb_hid.c                  # USB HID interface
│
├── LVGL/                           # LVGL source files
│   ├── src/
│   │   └── [LVGL core files]
│   └── lvgl.h
│
├── UI/
│   ├── ui_screens.c               # Auto-generated from XML export
│   ├── ui_events.c                # Event handlers
│   └── ui_events.h
│
└── Makefile (or STM32CubeIDE project)
```

### Key Integration Points

#### 1. Display Driver (`display_st7789.c`)

Implement `my_flush_cb()` from `embedded/main.c`:

```c
void my_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                 lv_color_t *color_p)
{
    int32_t w = area->x2 - area->x1 + 1;
    int32_t h = area->y2 - area->y1 + 1;
    
    // Set address window
    st7789_set_addr_window(area->x1, area->y1, area->x2, area->y2);
    
    // Send pixel data via SPI (with DMA for performance)
    st7789_send_bitmap(w * h, color_p);
    
    lv_disp_flush_ready(disp_drv);
}
```

**ST7789 Connection** (SPI2):
- **PB13** (SPI2_SCK) → SCK
- **PB15** (SPI2_MOSI) → SDA
- **PA11** → DC (Data/Command)
- **PA12** → CS (Chip Select)
- **PB12** → RESET

#### 2. Input Handlers

Each input source calls a handler:

```c
// In GPIO interrupt or input polling loop:

// Macro keys (GPIO pins PA0-PA9 or matrix scan)
if (key_pressed[i]) {
    handle_macro_key(i);
}

// Encoder 1 (PA8 A, PA9 B, PA10 push)
if (enc1_rotated_cw) {
    handle_encoder_rotation(0, +1);
}

// Tact buttons (PB0, PB1, PB2)
if (tact_btn1_pressed) {
    handle_tact_button_1();  // Mic mute
}
```

#### 3. Timer / Tick Management

LVGL requires a tick timer (1-5ms interval). Use SysTick or a hardware timer:

```c
// In SysTick_Handler or TIM_IRQHandler:
void SysTick_Handler(void)
{
    HAL_IncTick();
    lv_tick_inc(1);  // 1ms tick
}
```

#### 4. Main Loop

```c
int main(void)
{
    // Initialize hardware
    HAL_Init();
    SystemClockConfig();
    GPIO_Init();
    SPI_Init();
    USB_Init();
    Timer_Init();
    
    // Initialize LVGL
    lv_init();
    setup_display_driver();
    setup_input_driver();
    
    // Load UI
    ui_init(&g_device_state);
    ui_show_home();
    
    // Main loop
    while (1) {
        // Read inputs
        scan_macro_keys();
        update_encoders();
        check_tact_buttons();
        
        // LVGL update
        lv_task_handler();
        
        // Optional sleep
        // __WFI();
    }
}
```

### Memory Optimization

**STM32F103C8T6 Constraints**:
- RAM: 20 KB (shared with USB, FreeRTOS if used)
- Flash: 64 KB

**Recommended**:
- Use partial rendering buffer (1/10 of screen = 172 × 32 pixels = 11 KB)
- Enable LVGL's memory optimization flags
- Store fonts as compiled binary (not dynamic)
- Minimize animation use

**Buffer Configuration** (in `lv_conf.h`):
```c
#define LV_COLOR_DEPTH 16          // 16-bit color (5-6-5)
#define LV_MEM_SIZE (8 * 1024)     // 8 KB internal LVGL memory
#define LV_DISP_BUF_SIZE (DISP_HOR_RES * DISP_VER_RES / 10)
```

---

## Input Mapping Reference

See [`embedded/INPUT_MAPPING.h`](embedded/INPUT_MAPPING.h) for complete documentation.

### Quick Reference

```
PHYSICAL INPUT          EVENT FUNCTION              CONTEXT
─────────────────────────────────────────────────────────────
Macro Key 0-9          ui_event_macro_key(n)        Any screen
Encoder 1 CW           ui_event_encoder_rotate(0,+1) Depends on screen
Encoder 1 CCW          ui_event_encoder_rotate(0,-1) 
Encoder 1 Press        ui_event_encoder_press(0)    
Encoder 2 CW           ui_event_encoder_rotate(1,+1) 
Encoder 2 CCW          ui_event_encoder_rotate(1,-1) 
Encoder 2 Press        ui_event_encoder_press(1)    
Tact Button 1 (Mic)    ui_event_mic_toggle()        Global
Tact Button 2 (Prev)   ui_event_profile_prev()      Context
Tact Button 3 (Next)   ui_event_profile_next()      Context
```

---

## Build Instructions

### 1. Export LVGL Pro XML to C Code

In LVGL Pro:
1. File → Export
2. Select C code output
3. Choose target directory: `embedded/ui/`
4. Export generates:
   - `ui_scr_home.c`, `ui_scr_home.h`
   - `ui_scr_profiles.c`, `ui_scr_profiles.h`
   - ... (other screens)
   - `ui.c`, `ui.h` (main UI module)

### 2. Build Embedded Firmware

**Using STM32CubeIDE**:
1. File → New → STM32 Project
2. Select STM32F103C8Tx
3. Copy `Core/Src/main.c` from this project
4. Add LVGL sources to project
5. Add exported UI C files
6. Build → Build Project
7. Program to device via ST-Link

**Using Makefile**:
```bash
cd embedded
make clean
make all
# Output: firmware.bin (flash to device via STM32 Programmer)
```

### 3. Build PC Simulator

```bash
cd simulator
mkdir build && cd build
cmake ..
make
./streamdeck_simulator
```

---

## Acceptance Checklist

- [ ] **Profile Number Visible**
  - Home screen: Profile badge top-right ✓
  - Profiles screen: Profile badge top-right ✓
  - Macros screen: Profile badge top-right ✓
  - Encoders screen: Profile badge top-right ✓
  - RGB screen: Profile badge top-right ✓

- [ ] **Key Press Highlights**
  - Macro Grid shows 10 slots (2×5 layout) ✓
  - Pressing physical key 0-9 highlights corresponding slot ✓
  - "Last pressed" indicator updated with key name ✓
  - Highlight resets after 500ms ✓

- [ ] **Profile Switching**
  - [ button decreases profile (1→8 wraps) ✓
  - ] button increases profile (8→1 wraps) ✓
  - Profile selector screen displays all 8 options ✓
  - Rotating Enc1 navigates profile list ✓
  - Pressing Enc1 selects profile ✓

- [ ] **Encoder Assignment Screen**
  - Two sections: Encoder 1 and Encoder 2 ✓
  - Each shows current mode with selector list ✓
  - Modes available: Volume, Mic, Timeline, Zoom, Scroll ✓
  - Pressing encoder button applies selection ✓

- [ ] **Simulator Runs**
  - SDL window opens at 172×320 (scaled 2x) ✓
  - Keyboard input mapped correctly (1-9, 0, arrows, M, [ ]) ✓
  - Debug panel shows last 3 events ✓
  - FPS ~60 (smooth rendering) ✓
  - Can navigate all screens ✓

- [ ] **Code Quality**
  - XML is well-formed and LVGL Pro compatible ✓
  - C code compiles without errors ✓
  - Component reuse (badge, status bar, tiles) ✓
  - Consistent naming (scr_*, cmp_*, ui_event_*) ✓
  - Comprehensive documentation ✓

---

## Troubleshooting

### Simulator Won't Compile

**Issue**: SDL2 not found
- **Solution**: Install SDL2 dev package (see Prerequisites)

**Issue**: LVGL headers missing
- **Solution**: Ensure `lvgl/` folder exists in project root with full source

### Embedded Build Fails

**Issue**: `arm-none-eabi-gcc: command not found`
- **Solution**: Install ARM toolchain: https://developer.arm.com/downloads/-/gnu-rm

**Issue**: `error: declaration of 'lv_mem_alloc' has no prototype`
- **Solution**: Check LVGL version compatibility; ensure `lv_conf.h` is present

### Display is Black

**Issue**: Flush callback not implemented
- **Solution**: Implement `my_flush_cb()` with actual ST7789 SPI communication

**Issue**: Color data inverted or wrong endianness
- **Solution**: Check `LV_COLOR_DEPTH` and SPI byte order

### Encoders Not Responding

**Issue**: Encoder interrupt not firing
- **Solution**: Verify GPIO pin mapping and EXTI configuration

**Issue**: Debouncing too aggressive
- **Solution**: Reduce debounce timer; encoders may have missed steps

---

## Future Enhancements

- [ ] Add animations for screen transitions
- [ ] Implement micro SD card logging of macro history
- [ ] Add LCD backlight brightness control (PWM)
- [ ] Support wireless profile sync via BLE
- [ ] Web-based configuration interface
- [ ] Programmable animations on key press
- [ ] Gesture detection (swipe, long-press)
- [ ] Multi-language support

---

## References

- **LVGL Documentation**: https://docs.lvgl.io/
- **LVGL Pro**: https://lvgl.io/tools/lv_pro
- **STM32 HAL**: https://github.com/STMicroelectronics/STM32CubeF1
- **ST7789 Datasheet**: https://www.displayfuture.com/Display/Active%20Matrix%20TFT%20LCD%20Module/ST7789.pdf
- **EC11 Encoder Datasheet**: (Vendor specific)

---

## License

This project is provided as-is for embedded UI development.  
Includes LVGL (MIT License): https://github.com/lvgl/lvgl/blob/master/LICENSE

**Created**: January 2026  
**Last Updated**: January 2026

