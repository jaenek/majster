#ifndef _CONFIG_H
#define _CONFIG_H

typedef struct config_t {
	const char *filename;
	float w;
	float h;
} config_t;

config_t config_load();

#endif // _CONFIG_H
