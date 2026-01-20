#ifndef DEBUG_PANEL_H
#define DEBUG_PANEL_H

// Debug panel rendering and event logging
void debug_panel_init(void);
void debug_panel_render(void);
void debug_log_event(const char *format, ...);

#endif // DEBUG_PANEL_H
