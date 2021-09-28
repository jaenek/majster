#ifndef _RENDERER2D_H
#define _RENDERER2D_H

#include "types.h"
#include "camera.h"

void r2d_init(void);
void r2d_shutdown(void);
void r2d_begin_scene(camera_t *c);
void r2d_end_scene(void);
void r2d_set_clear_color(v4 col);
void r2d_draw_quad(v2 pos, v2 size, v4 color);
void r2d_draw_quad_tex(v2 pos, v2 size);

#endif // _RENDERER2D_H
