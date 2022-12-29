#pragma once
#include <stdint.h>

void gui_frontend_init(void);
void gui_frontend_draw_framebuffer(uint8_t framebuffer[32 * 64]);
void gui_frontend_cleanup(void);
