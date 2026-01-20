/**
 * LVGL Pro Configuration Header
 * Place this as lv_conf.h in the LVGL source directory
 */

#ifndef LV_CONF_H
#define LV_CONF_H

// ============================================================================
// DISPLAY SETTINGS
// ============================================================================

#define LV_HOR_RES_MAX          172
#define LV_VER_RES_MAX          320

#define LV_COLOR_DEPTH          16
#define LV_COLOR_16_SWAP        0

// ============================================================================
// MEMORY SETTINGS (STM32F103 optimized)
// ============================================================================

#define LV_MEM_SIZE             (8 * 1024)  // 8 KB internal memory
#define LV_MEM_CUSTOM           0

// ============================================================================
// RENDERING SETTINGS
// ============================================================================

#define LV_DISP_DEF_REFR_PERIOD 33          // ~30 FPS
#define LV_INDEV_DEF_READ_PERIOD 30

// Partial rendering (buffer 1/10 of screen)
#define LV_DISP_DEF_BUF_SIZE    (LV_HOR_RES_MAX * (LV_VER_RES_MAX / 10))

// ============================================================================
// FONT SETTINGS
// ============================================================================

#define LV_FONT_MONTSERRAT_10   1
#define LV_FONT_MONTSERRAT_11   1
#define LV_FONT_MONTSERRAT_12   1
#define LV_FONT_MONTSERRAT_13   1
#define LV_FONT_MONTSERRAT_14   1
#define LV_FONT_MONTSERRAT_16   1
#define LV_FONT_MONTSERRAT_18   1

#define LV_FONT_DEFAULT         &lv_font_montserrat_13

// ============================================================================
// FEATURE ENABLING (minimize for STM32F103)
// ============================================================================

#define LV_USE_OBJX             1
#define LV_USE_LABEL            1
#define LV_USE_BTN              1
#define LV_USE_CONT             1
#define LV_USE_BAR              1
#define LV_USE_SLIDER           1
#define LV_USE_ROLLER           1
#define LV_USE_BTNMATRIX        1
#define LV_USE_OBJMASK          0       // Disable for memory
#define LV_USE_SHADOW           0       // Optional: disable for performance
#define LV_USE_ANIMATION        1       // Keep: minimal animation support

// ============================================================================
// ANTI-ALIASING & DITHERING (disable for STM32F103)
// ============================================================================

#define LV_ANTIALIAS            0
#define LV_IMG_CF_INDEXED       0
#define LV_IMG_CF_ALPHA         0

// ============================================================================
// ADDITIONAL FEATURES
// ============================================================================

#define LV_USE_THEME_MATERIAL   0       // Too heavy
#define LV_USE_THEME_MONO       1       // Lightweight
#define LV_USE_IMG              0       // Minimal image support

#endif // LV_CONF_H
