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
#include "sokol_time.h"

#include "types.h"
#include "config.h"
#include "renderer2d.h"
#include "camera.h"
#include "events.h"

static struct {
	config_t config;
	u64 ticks;
	camera_t *camera;
} state;

void init(void) {
	sg_setup(&(sg_desc) { .context = sapp_sgcontext() });
	stm_setup();

	// that code will be in a lua init func
	r2d_init();
	state.camera = camera_init();
}

void frame(void) {
	// script

	// physics

	// render
	u64 start = stm_now();
	r2d_set_clear_color(V4(0.36f, 0.68f, 0.99f, 1.0f));
	r2d_begin_scene(state.camera);
	r2d_draw_quad(V2(0.0f, 0.0f), V2(50.0f, 50.0f), V4(1.0f, 1.0f, 1.0f, 1.0f));
	r2d_draw_quad_tex(V2(220.0f, 115.0f), V2(20.0f, 20.0f));
	r2d_draw_quad(V2(330.0f, 170.0f), V2(100.0f, 50.0f), V4(0.99f, 0.72f, 0.44f, 1.0f));
	r2d_end_scene();
	state.ticks = stm_laptime(&start);
}

void cleanup(void) {
	free(state.camera);
	r2d_shutdown();
	sg_shutdown();
}

sapp_desc sokol_main(int argc, char *argv[]) {
	state.config = config_load();

	return (sapp_desc) {
		.init_cb = init,
		.event_cb = event,
		.frame_cb = frame,
		.cleanup_cb = cleanup,
		.event_cb = event,
		.width = state.config.w,
		.height = state.config.h,
		.gl_force_gles2 = true,
		.window_title = "Hello world!",
	};
}
