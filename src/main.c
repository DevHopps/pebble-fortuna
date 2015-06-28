/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Benedict Balzer
 *
 */
#include <pebble.h>

static Window *s_main_window;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

static void update_time() {
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);

	static char buffer[] = "00:00";

	if(clock_is_24h_style() == true) {
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

	text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

static void main_window_load(Window *window) {
	//Logo layer
	GRect bounds = layer_get_bounds(window_get_root_layer(window));

	s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FORTUNA_PNG);

	s_bitmap_layer = bitmap_layer_create(bounds);
	bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
	bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));

	//Time layer
	s_time_layer = text_layer_create(GRect(0, -5, 144, 50));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);
	text_layer_set_text(s_time_layer, "00:00");

	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

	//Date layer
	s_date_layer = text_layer_create(GRect(0, 100, 144, 50));
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorBlack);
	text_layer_set_text(s_date_layer, "28. Jun 2015");

	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
	text_layer_destroy(s_time_layer);
	bitmap_layer_destroy(s_bitmap_layer);
  	gbitmap_destroy(s_bitmap);
}

static void init() {
	s_main_window = window_create();

	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	window_stack_push(s_main_window, true);

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	update_time();
}

static void deinit() {
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}