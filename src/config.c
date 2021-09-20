#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "config.h"

config_t new_config() { return (config_t){.filename = "src/config.lua", .w = 800, .h = 600}; }

void config_error(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	fprintf(stderr, "CONFIG[FATAL]: ");
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

float config_get_float(lua_State *L, const char *var) {
	int isnum;
	float result;
	lua_getglobal(L, var);
	result = (float)lua_tonumberx(L, -1, &isnum);
	if (!isnum) {
		config_error(L, "'%s' should be a number\n", var);
	}
	lua_pop(L, 1);
	return result;
}

config_t config_load() {
	char buff[256];
	int error;
	lua_State *L = luaL_newstate();

	// open and read from a configuration file
	config_t config = new_config();
	if (luaL_loadfile(L, config.filename) || lua_pcall(L, 0, 0, 0)) {
		config_error(L, "cannot run config. file: %s", lua_tostring(L, -1));
	}

	// get configuration variables
	config.w = config_get_float(L, "width");
	config.h = config_get_float(L, "height");

	lua_close(L);
	return config;
}
