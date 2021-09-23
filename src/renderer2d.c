#include "HandmadeMath.h"
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "shaders/flat.glsl.h"

#include "renderer2d.h"

// draw call limits
static const u32 MAX_QUADS = 10000;

typedef struct r2d_quad_t {
	v2 position;
	v2 size;
	v4 color;
} r2d_quad_t;

static struct {
	sg_buffer vbuf;
	sg_pipeline pip;
	sg_bindings bind;

	sg_color clear_color;
	vs_params_t uniforms;
	sg_image textures[2];

	i32 quad_count;
	r2d_quad_t *quad_vbuf_base;
	r2d_quad_t *quad_vbuf_cur;
} state;

void r2d_init(void) {
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
		.fs_images[SLOT_tex] = state.textures[1],
	};

	const float vertices[] = {
		// clang-format off
		// this is not compatible with D3D11 (look at texcube-sapp. sokol example)
		// X     Y     U     V
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		// clang-format on
	};

	state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
	    .data = SG_RANGE(vertices),
	    .label = "quad-verts",
	});

	const uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };

	state.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc) {
	    .type = SG_BUFFERTYPE_INDEXBUFFER,
	    .data = SG_RANGE(indices),
	    .label = "quad-indices",
	});

	state.bind.vertex_buffers[1] = sg_make_buffer(&(sg_buffer_desc) {
	    .size = MAX_QUADS * sizeof(r2d_quad_t),
	    .usage = SG_USAGE_STREAM,
	    .label = "quad-parameters",
	});

	state.quad_vbuf_base = calloc(MAX_QUADS, sizeof(r2d_quad_t));

	sg_shader shd = sg_make_shader(flat_shader_desc(sg_query_backend()));

	state.pip = sg_make_pipeline(&(sg_pipeline_desc){
	    .layout = {
			.buffers[1].step_func = SG_VERTEXSTEP_PER_INSTANCE,
		    .attrs = {
				[ATTR_vs_position] = { .format = SG_VERTEXFORMAT_FLOAT2, .buffer_index = 0},
				[ATTR_vs_texcoord] = { .format = SG_VERTEXFORMAT_FLOAT2, .buffer_index = 0},
				[ATTR_vs_inst_position] = { .format = SG_VERTEXFORMAT_FLOAT2, .buffer_index = 1},
				[ATTR_vs_size] = {.format = SG_VERTEXFORMAT_FLOAT2, .buffer_index = 1 },
				[ATTR_vs_color] = {.format = SG_VERTEXFORMAT_FLOAT4, .buffer_index = 1 },
			},
		},
	    .shader = shd,
	    .index_type = SG_INDEXTYPE_UINT16,
	    .cull_mode = SG_CULLMODE_BACK,
	    .depth = { .write_enabled = true, .compare = SG_COMPAREFUNC_LESS_EQUAL },
	    .label = "quad-pipeline",
	});
}

void r2d_shutdown(void) { free(state.quad_vbuf_base); }

void r2d_begin_scene(camera_t *c) {
	const float w = sapp_widthf();
	const float h = sapp_heightf();
	state.uniforms.vp = camera_get_vp(c);

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

	state.quad_vbuf_cur = state.quad_vbuf_base;
}

void r2d_end_scene(void) {
	sg_update_buffer(
	    state.bind.vertex_buffers[1], &(sg_range) {
					      .ptr = state.quad_vbuf_base,
					      .size = (size_t)state.quad_count * sizeof(r2d_quad_t),
					  });
	sg_draw(0, 6, state.quad_count);
	sg_end_pass();
	sg_commit();
}

void r2d_set_clear_color(v4 color) { state.clear_color = *(sg_color *)&color; }

void r2d_draw_quad(v2 pos, v2 size, v4 color) {
	state.quad_vbuf_cur->position = pos;
	state.quad_vbuf_cur->size = size;
	state.quad_vbuf_cur->color = color;
	state.quad_vbuf_cur++;

	state.quad_count += 1;
}
