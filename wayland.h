#ifndef WAYLAND_H
#define WAYLAND_H

#include <stdint.h>
#include "wayland-util.h"

enum {
	WL_EVENT_READABLE = 0x01,
	WL_EVENT_WRITEABLE = 0x02
};

struct wl_event_loop;
struct wl_event_source;
typedef void (*wl_event_loop_fd_func_t)(int fd, uint32_t mask, void *data);
typedef void (*wl_event_loop_idle_func_t)(void *data);

struct wl_event_loop *wl_event_loop_create(void);
void wl_event_loop_destroy(struct wl_event_loop *loop);
struct wl_event_source *wl_event_loop_add_fd(struct wl_event_loop *loop,
					     int fd, uint32_t mask,
					     wl_event_loop_fd_func_t func,
					     void *data);
int wl_event_loop_update_source(struct wl_event_loop *loop,
				struct wl_event_source *source,
				uint32_t mask);

int wl_event_loop_remove_source(struct wl_event_loop *loop,
				struct wl_event_source *source);
int wl_event_loop_wait(struct wl_event_loop *loop);
struct wl_event_source *wl_event_loop_add_idle(struct wl_event_loop *loop,
					       wl_event_loop_idle_func_t func,
					       void *data);

struct wl_client;

enum {
	WL_ARGUMENT_UINT32,
	WL_ARGUMENT_STRING,
	WL_ARGUMENT_OBJECT,
	WL_ARGUMENT_NEW_ID
};

struct wl_argument {
	uint32_t type;
	void *data;
};

struct wl_method {
	const char *name;
	void *func;
	int argument_count;
	const struct wl_argument *arguments;
};

struct wl_event {
	const char *name;
};

struct wl_interface {
	const char *name;
	int version;
	int method_count;
	const struct wl_method *methods;
	int event_count;
	const struct wl_event *events;
};

struct wl_object {
	const struct wl_interface *interface;
	uint32_t id;
};

struct wl_surface;
struct wl_display;

struct wl_map {
	int32_t x, y, width, height;
};

struct wl_event_loop *wl_display_get_event_loop(struct wl_display *display);

void wl_surface_set_data(struct wl_surface *surface, void *data);
void *wl_surface_get_data(struct wl_surface *surface);

struct wl_surface_iterator;
struct wl_surface_iterator *
wl_surface_iterator_create(struct wl_display *display, uint32_t mask);
int wl_surface_iterator_next(struct wl_surface_iterator *iterator,
			     struct wl_surface **surface);
void wl_surface_iterator_destroy(struct wl_surface_iterator *iterator);

struct wl_object *
wl_input_device_create(struct wl_display *display, const char *path);

void
wl_display_add_object(struct wl_display *display, struct wl_object *object);
int
wl_display_add_global(struct wl_display *display, struct wl_object *object);

void
wl_display_post_relative_event(struct wl_display *display,
			       struct wl_object *source, int dx, int dy);
void
wl_display_post_absolute_event(struct wl_display *display,
			       struct wl_object *source, int x, int y);
void
wl_display_post_button_event(struct wl_display *display,
			     struct wl_object *source, int button, int state);
void
wl_display_post_key_event(struct wl_display *display,
			  struct wl_object *source, int key, int state);

struct wl_compositor {
	const struct wl_compositor_interface *interface;
};

struct wl_compositor_interface {
	void (*notify_surface_create)(struct wl_compositor *compositor,
				      struct wl_surface *surface);
	void (*notify_surface_destroy)(struct wl_compositor *compositor,
				       struct wl_surface *surface);
	void (*notify_surface_attach)(struct wl_compositor *compositor,
				      struct wl_surface *surface,
				      uint32_t name, 
				      uint32_t width, uint32_t height,
				      uint32_t stride);
	void (*notify_surface_map)(struct wl_compositor *compositor,
				   struct wl_surface *surface,
				   struct wl_map *map);
	void (*notify_surface_copy)(struct wl_compositor *compositor,
				    struct wl_surface *surface,
				    int32_t dst_x, int32_t dst_y,
				    uint32_t name, uint32_t stride,
				    int32_t x, int32_t y,
				    int32_t width, int32_t height);
	void (*notify_surface_damage)(struct wl_compositor *compositor,
				      struct wl_surface *surface,
				      int32_t x, int32_t y,
				      int32_t width, int32_t height);
	void (*notify_pointer_motion)(struct wl_compositor *compositor,
				      struct wl_object *source,
				      int32_t x, int32_t y);
	void (*notify_key)(struct wl_compositor *compositor,
			   struct wl_object *source,
			   uint32_t key, uint32_t state);
};

void wl_display_set_compositor(struct wl_display *display,
			       struct wl_compositor *compositor);

struct wl_compositor *
wl_compositor_create(struct wl_display *display);

#endif
