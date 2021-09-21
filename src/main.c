#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"

#define SOKOL_IMPL
#if defined(_WIN32)
#define SOKOL_LOG(s) OutputDebugStringA(s)
#endif
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#include "types.h"
#include "config.h"
#include "renderer2d.h"

static struct { config_t config; } state;

void init(void) {
	sg_setup(&(sg_desc) { .context = sapp_sgcontext() });
	r2d_init();
}

void frame(void) {
	// script

	// physics

	// render
	r2d_begin_scene();
	r2d_draw_quad(V4(0.99f, 0.72f, 0.44f, 0.5f));
	r2d_end_scene();
}

void cleanup(void) {
	r2d_shutdown();
	sg_shutdown();
}

void event(const sapp_event *e) {
	if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
		if (e->key_code == SAPP_KEYCODE_ESCAPE) {
			sapp_request_quit();
		}
	}
}

sapp_desc sokol_main(int argc, char *argv[]) {
	state.config = config_load();

	return (sapp_desc) {
		.init_cb = init,
		.frame_cb = frame,
		.cleanup_cb = cleanup,
		.event_cb = event,
		.width = state.config.w,
		.height = state.config.h,
		.gl_force_gles2 = true,
		.window_title = "Hello world!",
	};
}
