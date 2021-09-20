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
#include "main.glsl.h"

#include "types.h"
#include "config.h"

static struct {
	float rx, ry;
	sg_pipeline pip;
	sg_bindings bind;
	config_t config;
} state;

void init(void) {
	sg_setup(&(sg_desc){.context = sapp_sgcontext()});

	float vertices[] = {
	    -1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0,	-1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
	    1.0,  1.0,	-1.0, 1.0, 0.0, 0.0, 1.0, -1.0, 1.0,  -1.0, 1.0, 0.0, 0.0, 1.0,

	    -1.0, -1.0, 1.0,  0.0, 1.0, 0.0, 1.0, 1.0,	-1.0, 1.0,  0.0, 1.0, 0.0, 1.0,
	    1.0,  1.0,	1.0,  0.0, 1.0, 0.0, 1.0, -1.0, 1.0,  1.0,  0.0, 1.0, 0.0, 1.0,

	    -1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 1.0, -1.0, 1.0,  -1.0, 0.0, 0.0, 1.0, 1.0,
	    -1.0, 1.0,	1.0,  0.0, 0.0, 1.0, 1.0, -1.0, -1.0, 1.0,  0.0, 0.0, 1.0, 1.0,

	    1.0,  -1.0, -1.0, 1.0, 0.5, 0.0, 1.0, 1.0,	1.0,  -1.0, 1.0, 0.5, 0.0, 1.0,
	    1.0,  1.0,	1.0,  1.0, 0.5, 0.0, 1.0, 1.0,	-1.0, 1.0,  1.0, 0.5, 0.0, 1.0,

	    -1.0, -1.0, -1.0, 0.0, 0.5, 1.0, 1.0, -1.0, -1.0, 1.0,  0.0, 0.5, 1.0, 1.0,
	    1.0,  -1.0, 1.0,  0.0, 0.5, 1.0, 1.0, 1.0,	-1.0, -1.0, 0.0, 0.5, 1.0, 1.0,

	    -1.0, 1.0,	-1.0, 1.0, 0.0, 0.5, 1.0, -1.0, 1.0,  1.0,  1.0, 0.0, 0.5, 1.0,
	    1.0,  1.0,	1.0,  1.0, 0.0, 0.5, 1.0, 1.0,	1.0,  -1.0, 1.0, 0.0, 0.5, 1.0,
	};

	sg_buffer vbuf = sg_make_buffer(&(sg_buffer_desc){
	    .data = SG_RANGE(vertices),
	    .label = "cube-verts",
	});

	uint16_t indices[] = {
	    0,	1,  2,	0,  2,	3,  6,	5,  4,	7,  6,	4,  8,	9,  10, 8,  10, 11,
	    14, 13, 12, 15, 14, 12, 16, 17, 18, 16, 18, 19, 22, 21, 20, 23, 22, 20,
	};

	sg_buffer ibuf = sg_make_buffer(&(sg_buffer_desc){
	    .type = SG_BUFFERTYPE_INDEXBUFFER,
	    .data = SG_RANGE(indices),
	    .label = "cube-indices",
	});

	sg_shader shd = sg_make_shader(main_shader_desc(sg_query_backend()));

	state.pip = sg_make_pipeline(&(sg_pipeline_desc){
	    .layout =
		{
		    .buffers[0].stride = 28,
		    .attrs =
			{
			    [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
			    [ATTR_vs_color0].format = SG_VERTEXFORMAT_FLOAT4,
			},
		},
	    .shader = shd,
	    .index_type = SG_INDEXTYPE_UINT16,
	    .cull_mode = SG_CULLMODE_BACK,
	    .depth = {.write_enabled = true, .compare = SG_COMPAREFUNC_LESS_EQUAL},
	    .label = "cube-pipeline",
	});

	state.bind = (sg_bindings){.vertex_buffers[0] = vbuf, .index_buffer = ibuf};
}

void frame(void) {
	// script

	// physics

	// render
	vs_params_t vs_params;
	const float w = state.config.w;
	const float h = state.config.h;
	m4 proj = HMM_Perspective(60.0f, w / h, 0.01f, 10.0f);
	m4 view = HMM_LookAt(V3(0.0f, 1.5f, 6.0f), V3(0.0f, 0.0f, 0.0f), V3(0.0f, 1.0f, 0.0f));
	m4 view_proj = HMM_MultiplyMat4(proj, view);
	state.rx += 1.0f;
	state.ry += 2.0f;
	m4 rxm = HMM_Rotate(state.rx, V3(1.0f, 0.0f, 0.0f));
	m4 rym = HMM_Rotate(state.ry, V3(0.0f, 1.0f, 0.0f));
	m4 model = HMM_MultiplyMat4(rxm, rym);
	vs_params.mvp = HMM_MultiplyMat4(view_proj, model);

	sg_pass_action pass_action = {
	    .colors[0] = {.action = SG_ACTION_CLEAR, .value = {0.25f, 0.5f, 0.75f, 1.0f}},
	};
	sg_begin_default_pass(&pass_action, (int)w, (int)h);
	sg_apply_pipeline(state.pip);
	sg_apply_bindings(&state.bind);
	sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(vs_params));
	sg_draw(0, 36, 1);
	sg_end_pass();
	sg_commit();
}

void cleanup(void) { sg_shutdown(); }

void event(const sapp_event *e) {
	if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
		if (e->key_code == SAPP_KEYCODE_ESCAPE) {
			sapp_request_quit();
		}
	}
}

sapp_desc sokol_main(int argc, char *argv[]) {
	state.config = config_load();

	return (sapp_desc){
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
