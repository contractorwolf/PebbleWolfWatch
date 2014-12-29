#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_label_layer;
static TextLayer *s_second_layer;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  
  
  static char secs[] = "00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);

  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    
  }
  
  strftime(secs, sizeof("00"), "%S", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  
  text_layer_set_text(s_second_layer, secs);
}

static void AddTextLayer(Window *window,TextLayer *text_layer,const char *text,const char *font_key,GColor background,GColor text_color, GTextAlignment alignment){
  text_layer_set_background_color(text_layer, background);
  text_layer_set_text_color(text_layer, text_color);
  text_layer_set_text(text_layer, text);
  text_layer_set_font(text_layer, fonts_get_system_font(font_key));
  text_layer_set_text_alignment(text_layer, alignment);  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 25, 144, 50));
  s_second_layer = text_layer_create(GRect(0, 77, 130, 40));
  s_label_layer = text_layer_create(GRect(0, 138, 144, 50));
  

  AddTextLayer(window,s_time_layer, "00:00",FONT_KEY_ROBOTO_BOLD_SUBSET_49,GColorBlack,GColorWhite, GTextAlignmentCenter);
  AddTextLayer(window,s_second_layer, "00",FONT_KEY_GOTHIC_28,GColorBlack,GColorWhite, GTextAlignmentRight);
  AddTextLayer(window,s_label_layer,"the wolf",FONT_KEY_GOTHIC_28,GColorBlack,GColorWhite,GTextAlignmentCenter);
  
  
  // Make sure the time is displayed from the start
  update_time();
}



static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_label_layer);
  text_layer_destroy(s_second_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  window_set_background_color(s_main_window, GColorBlack);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
