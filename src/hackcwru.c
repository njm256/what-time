#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create(
        GRect((int) (.1 * bounds.size.w),
              (int) (.18 * bounds.size.h),
              (int) (.8 * bounds.size.w),
              (int) (.7 * bounds.size.h)));

    text_layer_set_background_color(text_layer, GColorWhite);
    text_layer_set_text_color(text_layer, GColorBlack);
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_text(text_layer, "\nTime to get a watch :P");
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

    layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void main_window_unload(Window *window) {
    text_layer_destroy(text_layer);
}


static void init() {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    window_stack_push(window, true);
}

static void deinit() {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
