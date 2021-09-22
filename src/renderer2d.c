#include "HandmadeMath.h"
#include "renderer2d.h"
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "shaders/flat.glsl.h"

#include "types.h"

static struct {
	sg_pipeline pip;
	sg_bindings bind;
	vs_params_t vs_params;
} state;

void r2d_init() {
	float vertices[] = {
		// clang-format off
		// X      Y     Z
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		// clang-format on
	};

	sg_buffer vbuf = sg_make_buffer(&(sg_buffer_desc) {
	    .data = SG_RANGE(vertices),
	    .label = "quad-verts",
	});

	uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };

	sg_buffer ibuf = sg_make_buffer(&(sg_buffer_desc) {
	    .type = SG_BUFFERTYPE_INDEXBUFFER,
	    .data = SG_RANGE(indices),
	    .label = "quad-indices",
	});

	sg_shader shd = sg_make_shader(flat_shader_desc(sg_query_backend()));

	state.pip = sg_make_pipeline(&(sg_pipeline_desc){
	    .layout = {
		    .buffers[0].stride = 0,
		    .attrs = { [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3 },
		},
	    .shader = shd,
	    .index_type = SG_INDEXTYPE_UINT16,
	    .cull_mode = SG_CULLMODE_BACK,
	    .depth = { .write_enabled = true, .compare = SG_COMPAREFUNC_LESS_EQUAL },
	    .label = "quad-pipeline",
	});

	state.bind = (sg_bindings) { .vertex_buffers[0] = vbuf, .index_buffer = ibuf };
}

void r2d_shutdown() {}

void r2d_begin_scene() {
	const float w = sapp_widthf();
	const float h = sapp_heightf();
	struct camera {
		v3 pos;
		m4 rot;
	} camera = {};
	camera.rot = HMM_Rotate(0.0f, V3(0.0f, 0.0f, 1.0f));
	m4 xform = HMM_MultiplyMat4(HMM_Translate(camera.pos), camera.rot);
	state.vs_params.vp = HMM_MultiplyMat4(HMM_Orthographic(-1.2f, 1.2f, -0.9f, 0.9f, -1.0f, 1.0f), xform);

	sg_pass_action pass_action = {
		.colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.25f, 0.5f, 0.75f, 1.0f } },
	};
	sg_begin_default_pass(&pass_action, (int)w, (int)h);
	sg_apply_pipeline(state.pip);
	sg_apply_bindings(&state.bind);
}

void r2d_end_scene() {
	sg_end_pass();
	sg_commit();
}

void r2d_draw_quad(v4 color) {
	state.vs_params.flat_color = color;
	sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(state.vs_params));
	sg_draw(0, 6, 1);
}