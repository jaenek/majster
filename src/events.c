#include <assert.h>
#include <stdbool.h>

#include "sokol_app.h"

#include "event.h"

static bool is_key_event(sapp_event_type t) {
	switch (t) {
	case SAPP_EVENTTYPE_KEY_DOWN:
	case SAPP_EVENTTYPE_KEY_UP:
	case SAPP_EVENTTYPE_CHAR:
		return true;
	default:
		return false;
	}
}

static bool is_mouse_or_drop_event(sapp_event_type t) {
	switch (t) {
	case SAPP_EVENTTYPE_MOUSE_DOWN:
	case SAPP_EVENTTYPE_MOUSE_UP:
	case SAPP_EVENTTYPE_MOUSE_SCROLL:
	case SAPP_EVENTTYPE_MOUSE_MOVE:
	case SAPP_EVENTTYPE_MOUSE_ENTER:
	case SAPP_EVENTTYPE_MOUSE_LEAVE:
	case SAPP_EVENTTYPE_FILES_DROPPED:
		return true;
	default:
		return false;
	}
}

static bool is_touch_event(sapp_event_type t) {
	switch (t) {
	case SAPP_EVENTTYPE_TOUCHES_BEGAN:
	case SAPP_EVENTTYPE_TOUCHES_MOVED:
	case SAPP_EVENTTYPE_TOUCHES_ENDED:
	case SAPP_EVENTTYPE_TOUCHES_CANCELLED:
		return true;
	default:
		return false;
	}
}

typedef struct event_t {
	sapp_event event;
} event_t;

static struct {
	event_t events[_SAPP_EVENTTYPE_NUM];
} state;

void event(const sapp_event *e) {
	assert((e->type >= 0) && (e->type < _SAPP_EVENTTYPE_NUM));
	state.events[e->type].event = *e;
}
