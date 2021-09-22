#ifndef _CAMERA_H
#define _CAMERA_H

#include "types.h"

typedef struct camera_t {
	v3 pos;
	m4 rot;
	m4 proj;
	m4 vp;
} camera_t;

camera_t camera_init();
void camera_set_pos(camera_t *camera, v3 pos);

#endif // _CAMERA_H
