#define SOKOL_SPRITEBATCH_IMPL
#define CAMERA_WIDTH (480)
#define CAMERA_HEIGHT (270)

#include "HandmadeMath.h"
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_spritebatch.h"
#include "shaders/flat.glsl.h"

#include "renderer2d.h"

static struct {
	int screen_width;
	int screen_height;
	int gameplay_quad_width;
	int gameplay_quad_height;
	int gameplay_quad_x;
	int gameplay_quad_y;

	sg_color clear_color;
	sg_image base_atlas;

	sg_pass_action pass_action;

	sg_pass gameplay_pass;
	sbatch_context gameplay_context;
	sbatch_pipeline gameplay_pipeline;
	sg_image gameplay_render_target;

	sbatch_context output_context;
	sbatch_pipeline output_pipeline;
} state;

void r2d_init(void) {
	// renderer base atlas
	u32 pixels[] = {
		// clang-format off
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD,
		0xDEADBEEF, 0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF,
		0xDEADBEEF, 0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD,
		0xDEADBEEF, 0xFFDDDDDD, 0xFFFFFFFF, 0xFFDDDDDD, 0xFFFFFFFF,
		// clang-format on
	};

	state.base_atlas = sg_alloc_image();
	sg_init_image(state.base_atlas, &(sg_image_desc) { .label = "r2d_base-atlas",
					    .width = 5,
					    .height = 4,
					    .pixel_format = SG_PIXELFORMAT_RGBA8,
					    .min_filter = SG_FILTER_NEAREST,
					    .mag_filter = SG_FILTER_NEAREST,
					    .wrap_u = SG_WRAP_REPEAT,
					    .wrap_v = SG_WRAP_REPEAT,
					    .data.subimage[0][0] = {
						.ptr = pixels,
						.size = (size_t)(5 * 4 * 4),
					    } });
	sbatch_setup(&(sbatch_desc) { 0 });

	state.gameplay_render_target = sg_make_image(&(sg_image_desc) {
	    .label = "r2d_gameplay-render-target",
	    .render_target = true,
	    .width = CAMERA_WIDTH,
	    .height = CAMERA_HEIGHT,
	    .min_filter = SG_FILTER_NEAREST,
	    .mag_filter = SG_FILTER_NEAREST,
	    .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
	    .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
	});

	state.gameplay_pass = sg_make_pass(&(sg_pass_desc) {
	    .color_attachments[0].image = state.gameplay_render_target,
	    .label = "r2d_gameplay-pass",
	});

	state.gameplay_context = sbatch_make_context(&(sbatch_context_desc) {
	    .label = "r2d_gameplay-context",
	});

	state.gameplay_pipeline = sbatch_make_pipeline(&(sg_pipeline_desc) {
	    .depth.pixel_format = SG_PIXELFORMAT_NONE,
	    .label = "r2d_gameplay-pipeline",
	});

	state.screen_height = sapp_height();
	state.screen_width = sapp_width();
	const float aspect = (float)CAMERA_WIDTH / (float)CAMERA_HEIGHT;

	state.gameplay_quad_width = state.screen_width;
	state.gameplay_quad_height = (int)((float)state.gameplay_quad_width / aspect + 0.5f);

	if (state.gameplay_quad_height > state.screen_height) {
		state.gameplay_quad_height = state.screen_height;
		state.gameplay_quad_width =
		    (int)((float)state.gameplay_quad_height * aspect + 0.5f);
	}

	state.gameplay_quad_x = (state.screen_width / 2) - (state.gameplay_quad_width / 2);
	state.gameplay_quad_y = (state.screen_height / 2) - (state.gameplay_quad_height / 2);

	state.output_context = sbatch_make_context(&(sbatch_context_desc) {
	    .max_sprites_per_frame = 1,
	    .label = "r2d_output-context",
	});

	state.output_pipeline = sbatch_make_pipeline(&(sg_pipeline_desc) {
	    .label = "r2d_output-pipeline",
	});
}

void r2d_shutdown(void) {}

void r2d_begin_scene(camera_t *c) {
	sg_begin_pass(state.gameplay_pass, &state.pass_action);
	sbatch_begin(state.gameplay_context, &(sbatch_render_state) {
						 .pipeline = state.gameplay_pipeline,
						 .use_pixel_snap = true,
						 .canvas_width = CAMERA_WIDTH,
						 .canvas_height = CAMERA_HEIGHT,
					     });
}

void r2d_end_scene(void) {
	sbatch_end();
	sg_end_pass();

	sg_begin_default_pass(&state.pass_action, state.screen_width, state.screen_height);
	sbatch_begin(state.output_context, &(sbatch_render_state) {
					       .pipeline = state.output_pipeline,
					       .canvas_width = state.screen_width,
					       .canvas_height = state.screen_height,
					   });

	sbatch_push_sprite_rect(&(sbatch_sprite_rect) {
			.image = state.gameplay_render_target,
			.destination = {
				.x = (float)state.gameplay_quad_x,
				.y = (float)state.gameplay_quad_y,
				.width = (float)state.gameplay_quad_width,
				.height = (float)state.gameplay_quad_height,
			}

	});
	sbatch_end();
	sg_end_pass();
	sg_commit();
	sbatch_frame();
}

void r2d_set_clear_color(v4 color) {
	state.pass_action = (sg_pass_action){
		.colors[0] = {
			.action = SG_ACTION_CLEAR,
		    .value = *(sg_color *)&color,
		},
	};
}

void r2d_draw_quad(v2 pos, v2 size, v4 color) {
	sbatch_push_sprite_rect(&(sbatch_sprite_rect) {
			.image = state.base_atlas,
			.destination = {
				.x = pos.X,
				.y = pos.Y,
				.width = size.X,
				.height = size.Y,
			},
			.source = {
				.x = 0,
				.y = 0,
				.width = 1,
				.height = 1,
			},
			.color = (sg_color *)&color,
	});
}

void r2d_draw_quad_tex(v2 pos, v2 size) {
	sbatch_push_sprite_rect(&(sbatch_sprite_rect) {
			.image = state.base_atlas,
			.destination = {
				.x = pos.X,
				.y = pos.Y,
				.width = size.X,
				.height = size.Y,
			},
			.source = {
				.x = 1,
				.y = 0,
				.width = 4,
				.height = 4,
			},
	});
}
