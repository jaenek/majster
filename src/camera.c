#include <assert.h>

#include "camera.h"

camera_t *camera_init(void) {
	camera_t *c = malloc(sizeof(camera_t));
	assert(c);

	const f32 a_w = 1.2f, a_h = 0.9f;
	c->pos = V3(0.0f, 0.0f, 0.0f);
	c->rot = HMM_Rotate(0.0f, V3(0.0f, 0.0f, 1.0f));
	c->proj = HMM_Orthographic(-a_w, a_w, -a_h, a_h, -1.0f, 1.0f);
	return c;
}

void camera_rotate(camera_t *c, f32 angle) {
	c->angle += angle;
	c->rot = HMM_Rotate(c->angle, V3(0.0f, 0.0f, 1.0f));
}

m4 camera_get_vp(camera_t *c) {
	const m4 view = HMM_MultiplyMat4(HMM_Translate(c->pos), c->rot);
	return HMM_MultiplyMat4(c->proj, view);
}
