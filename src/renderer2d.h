#ifndef _RENDERER2D_H
#define _RENDERER2D_H

#include "types.h"

void r2d_init();
void r2d_shutdown();
void r2d_begin_scene();
void r2d_end_scene();
void r2d_set_clear_color(v4 col);
void r2d_set_color(v4 col);
void r2d_draw_quad();

#endif // _RENDERER2D_H
