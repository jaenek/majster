#include "HandmadeMath.h"
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "shaders/flat.glsl.h"

#include "renderer2d.h"

typedef struct r2d_quad_t {
	m4 xform;
} r2d_quad_t;

static struct {
	sg_pipeline pip;
	sg_bindings bind;
	sg_color clear_color;
	vs_params_t uniforms;
	sg_image textures[2];
} state;

void r2d_init(void) {
	float vertices[] = {
		// clang-format off
		// X      Y     Z
		// this is not compatible with D3D11 (look at texcube-sapp. sokol example)
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
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
		    .attrs = {
				[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
				[ATTR_vs_texcoord].format = SG_VERTEXFORMAT_FLOAT2,
			},
		},
	    .shader = shd,
	    .index_type = SG_INDEXTYPE_UINT16,
	    .cull_mode = SG_CULLMODE_BACK,
	    .depth = { .write_enabled = true, .compare = SG_COMPAREFUNC_LESS_EQUAL },
	    .label = "quad-pipeline",
	});

	// blank texture - single white pixel
	u32 pixel[] = { 0xFFFFFFFF };
	state.textures[0] = sg_make_image(&(sg_image_desc) {
	    .width = 1,
	    .height = 1,
	    .data.subimage[0][0] = SG_RANGE(pixel),
	    .label = "quad-blank_texture",
	});
	u32 pixels[] = {
		// clang-format off
		0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD,
		0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD,
		0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF,
		// clang-format on
	};
	state.textures[1] = sg_make_image(&(sg_image_desc) {
	    .width = 4,
	    .height = 4,
	    .data.subimage[0][0] = SG_RANGE(pixels),
	    .label = "quad-checkerboard_texture",
	});

	state.bind = (sg_bindings) {
		.vertex_buffers[0] = vbuf,
		.index_buffer = ibuf,
		.fs_images[SLOT_tex] = state.textures[1],
	};
}

void r2d_shutdown(void) {}

void r2d_begin_scene(camera_t *c) {
	const float w = sapp_widthf();
	const float h = sapp_heightf();
	state.uniforms.vp = camera_get_vp(c);
	state.uniforms.flat_color = V4(1.0f, 1.0f, 1.0f, 1.0f);

	sg_pass_action pass_action = {
		.colors[0] = {
			.action = SG_ACTION_CLEAR,
		    .value = state.clear_color,
		},
	};
	sg_begin_default_pass(&pass_action, (int)w, (int)h);
	sg_apply_pipeline(state.pip);
	sg_apply_bindings(&state.bind);
	sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(state.uniforms));
}

void r2d_end_scene(void) {
	sg_end_pass();
	sg_commit();
}

void r2d_set_clear_color(v4 color) { state.clear_color = *(sg_color *)&color; }

void r2d_set_color(v4 color) { state.uniforms.flat_color = color; }

// that needs to add a new quad entry to a quad list with
// a transform of a new quad
void r2d_draw_quad(v2 pos) {
	state.uniforms.transform = HMM_Translate(V3(pos.X, pos.Y, 0.0f));
	sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(state.uniforms));
	sg_draw(0, 6, 1); // this needs to take a quad count from a list
}
