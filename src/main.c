/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Benedict Balzer
 *
 */
#include <pebble.h>

static void init() {

}

static void deinit() {

}

/*
 * 
 */
int main(void) {
  init();
  app_event_loop();
  deinit();
}