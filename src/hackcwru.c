#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

const char *hour_int_to_str(int hour) {
    const char *str = NULL;
    switch(hour) {
        case 1:
            str = "one";
            break;
        case 2:
            str = "two";
            break;
        case 3:
            str = "three";
            break;
        case 4:
            str = "four";
            break;
        case 5:
            str = "five";
            break;
        case 6:
            str = "six";
            break;
        case 7:
            str = "seven";
            break;
        case 8:
            str = "eight";
            break;
        case 9:
            str = "nine";
            break;
        case 10:
            str = "ten";
            break;
        case 11:
            str = "eleven";
            break;
        case 12:
            str = "twelve";
            break;
        default:
            str = "I don't know";
    }
    return str;
}
void gen_msg(const char *buf, size_t buf_len, int hour, int min) {
    if (min >= 0 && min <= 10) {
        strncpy(buf, "It's ", 63); 
        strncat(buf, hour_int_to_str(hour), 63);
    }
    else if (min > 10 && min <= 20) {
        str = "It's a quarter after.";
        strncpy(buf, "It's a quarter after.", 63);
    }
    else if (min >= 20 && min <= 27) {
        strncpy(buf, "It's almost half past ", 63);
        strncat(buf, hour_int_to_str(hour), 63);
    }
    else if (min >= 28 && min <=32) {
        strncpy(buf, "It's half past ", 63);
        strncat(buf, hour_int_to_str(hour), 63);
    }
    else if (min >= 33 && min <= 39) {
        strncpy(buf, "It's ", 63);
        strncat(buf, hour_int_to_str(hour), 63);
        strncat(buf, " thirty-something.", 63);
    }
    else if (min >= 40 & min <= 50) {
        strncpy(buf, "It's a quarter to ", 63);
        strncat(buf, hour_int_to_str(hour % 12 + 1), 63);
    }
    else {
        strncpy(buf, "It's almost ", 63);
        strncat(buf, hour_int_to_str(hour % 12 + 1), 63);
    }
}

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    static char buf[64];
    strftime(buf, sizeof(buf), "\n%I:%M", tick_time);
    int hour, min;
    sscanf(buf, "\n%d:%d", &hour, &min);
    gen_msg(buf, sizeof(buf), hour, min);

    text_layer_set_text(text_layer, buf);

}

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

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void init() {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = main_window_load,
            .unload = main_window_unload
            });
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

    window_stack_push(window, true);
    update_time();
}

static void deinit() {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
