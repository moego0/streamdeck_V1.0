# LVGL Pro UI - Styles Reference

## Color Palette

### Dark Theme (Default)
```
dark_bg:        #0a0e27 (deep blue-black)
dark_fg:        #e8e8e8 (light gray)
dark_surface:   #1a1f3a (lighter blue-black)
dark_border:    #2a2f4a (medium blue-black)
```

### Light Theme
```
light_bg:       #f5f5f5 (light gray)
light_fg:       #1a1a1a (dark gray/black)
light_surface:  #ffffff (white)
light_border:   #e0e0e0 (light gray)
```

### Accent Colors (User-selectable)
```
accent_blue:    #1e90ff (default)
accent_cyan:    #00bfff
accent_green:   #00ff7f
accent_orange:  #ff8c00
accent_red:     #ff1744
```

### Status Colors
```
usb_connected:      #00ff7f (green)
usb_disconnected:   #ff1744 (red)
mic_active:         #00ff7f (green)
mic_muted:          #ff8c00 (orange)
```

## Fonts

All fonts are built-in LVGL fonts to minimize flash usage:

- **Montserrat 10pt** - Key labels in grid
- **Montserrat 11pt** - Small text, hints
- **Montserrat 12pt** - Status text, buttons
- **Montserrat 13pt** - Regular labels
- **Montserrat 14pt** - Tile titles
- **Montserrat 16pt** - Screen titles
- **Montserrat 18pt** - Profile badge number

## Style Classes

### Profile Badge
```
style_class: "profile_badge_bg"
  - bg_color: accent_blue
  - text_color: white
  - radius: 8
  - pad_all: 4
  - border_width: 1
  - border_color: accent_cyan

style_class: "profile_number_text"
  - text_color: white
  - text_font: lv_font_montserrat_18
  - text_align: center
```

### Status Bar
```
style_class: "status_bar_bg"
  - bg_color: dark_surface
  - bg_opa: 240
  - text_color: dark_fg
  - pad_all: 4
  - border_width: 1
  - border_color: dark_border

style_class: "status_icon"
  - text_color: accent_cyan
  - text_font: lv_font_montserrat_12
  - text_align: center

style_class: "status_text"
  - text_color: dark_fg
  - text_font: lv_font_montserrat_11
  - text_align: center
```

### Tile Widget
```
style_class: "tile_bg"
  - bg_color: dark_surface
  - bg_opa: 255
  - text_color: dark_fg
  - radius: 6
  - pad_all: 8
  - border_width: 1
  - border_color: dark_border

style_class: "tile_title"
  - text_color: accent_cyan
  - text_font: lv_font_montserrat_14
  - text_align: left

style_class: "tile_bar"
  - bg_color: dark_border
  - indicator_color: accent_blue
  - radius: 3

style_class: "tile_value_text"
  - text_color: dark_fg
  - text_font: lv_font_montserrat_12
  - text_align: left
```

### Macro Key Slots
```
style_class: "macro_key_slot"
  - bg_color: dark_border
  - bg_opa: 200
  - radius: 4
  - pad_all: 2
  - border_width: 1
  - border_color: accent_blue
  - text_color: dark_fg

style_class: "macro_key_slot:pressed"
  - bg_color: accent_blue
  - bg_opa: 255
  - border_color: accent_cyan
  - shadow_width: 4
  - shadow_color: accent_blue

style_class: "key_slot_label"
  - text_color: dark_fg
  - text_font: lv_font_montserrat_10
  - text_align: center

style_class: "key_color_indicator"
  - bg_color: accent_blue
  - radius: 2
```

### Screens
```
style_class: "screen_bg"
  - bg_color: dark_bg
  - text_color: dark_fg

style_class: "screen_title"
  - text_color: accent_cyan
  - text_font: lv_font_montserrat_16
  - text_align: center

style_class: "screen_label"
  - text_color: dark_fg
  - text_font: lv_font_montserrat_13
```

### Buttons
```
style_class: "btn_primary"
  - bg_color: accent_blue
  - text_color: white
  - text_font: lv_font_montserrat_12
  - radius: 6
  - pad_all: 8
  - border_width: 1
  - border_color: accent_cyan

style_class: "btn_primary:pressed"
  - bg_color: accent_cyan
  - shadow_width: 3
  - shadow_color: accent_blue

style_class: "btn_secondary"
  - bg_color: dark_border
  - text_color: dark_fg
  - text_font: lv_font_montserrat_11
  - radius: 4
  - pad_all: 6
```

### Lists
```
style_class: "list_item"
  - bg_color: dark_surface
  - text_color: dark_fg
  - text_font: lv_font_montserrat_12
  - pad_all: 6
  - border_width: 0

style_class: "list_item:selected"
  - bg_color: accent_blue
  - text_color: white
  - radius: 4
  - border_width: 2
  - border_color: accent_cyan
```

### Sliders
```
style_class: "slider_main"
  - bg_color: dark_border
  - indicator_color: accent_blue
  - radius: 6
```

### Quick Overlay
```
style_class: "overlay_bg"
  - bg_color: dark_bg
  - bg_opa: 200
  - radius: 8
  - pad_all: 12
  - border_width: 2
  - border_color: accent_blue
  - shadow_width: 4
  - shadow_color: accent_blue

style_class: "overlay_text"
  - text_color: accent_cyan
  - text_font: lv_font_montserrat_14
  - text_align: center
```

## Implementation Notes

Styles can be applied in LVGL Editor by:
1. Selecting an object
2. Adding style properties in the properties panel
3. Or by referencing a style_class name if supported by your LVGL version

For LVGL Pro projects, styles are typically defined directly on each object or in a shared style file referenced by screens.

When exporting to C code, LVGL will generate style definitions automatically based on the XML attributes.
