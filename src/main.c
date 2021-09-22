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
	r2d_draw_quad(V2(-1.0f, -1.0f));
	r2d_set_color(V4(0.99f, 0.72f, 0.44f, 1.0f));
	r2d_draw_quad(V2(0.0f, 0.0f));
	r2d_end_scene();
	state.ticks = stm_laptime(&start);
}

void cleanup(void) {
	free(state.camera);
	r2d_shutdown();
	sg_shutdown();
}

void event(const sapp_event *e) {
	if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
		switch (e->key_code) {
		case SAPP_KEYCODE_ESCAPE: {
			sapp_request_quit();
			break;
		}
		case SAPP_KEYCODE_Q: {
			camera_rotate(state.camera, 10.0f);
			break;
		}
		case SAPP_KEYCODE_E: {
			camera_rotate(state.camera, -10.0f);
			break;
		}
		case SAPP_KEYCODE_D: {
			state.camera->pos.X += 0.01f;
			break;
		}
		case SAPP_KEYCODE_A: {
			state.camera->pos.X -= 0.01f;
			break;
		}
		case SAPP_KEYCODE_W: {
			state.camera->pos.Y += 0.01f;
			break;
		}
		case SAPP_KEYCODE_S: {
			state.camera->pos.Y -= 0.01f;
			break;
		}
		default:
			break;
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
