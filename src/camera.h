#ifndef _CAMERA_H
#define _CAMERA_H

#include "types.h"

typedef struct camera_t {
	v3 pos;
	float angle;
	m4 rot;
	m4 proj;
} camera_t;

camera_t *camera_init(void);
void camera_rotate(camera_t *camera, f32 angle);
m4 camera_get_vp(camera_t *camera);

#endif // _CAMERA_H
