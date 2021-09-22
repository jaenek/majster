#include "camera.h"

camera_t camera_init() {
	const float a_w = 1.2f, a_h = 0.9f;
	const m4 rot = HMM_Rotate(0.0f, V3(0.0f, 0.0f, 1.0f));
	const m4 proj = HMM_Orthographic(-a_w, a_w, -a_h, a_h, -1.0f, 1.0f);
	return (camera_t) {
		.rot = rot,
		.proj = proj,
		.vp = HMM_MultiplyMat4(proj, rot),
	};
}

void camera_set_pos(camera_t *c, v3 pos) {
	const m4 view = HMM_MultiplyMat4(HMM_Translate(c->pos), c->rot);
	c->vp = HMM_MultiplyMat4(c->proj, view);
}
