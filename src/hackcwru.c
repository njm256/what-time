#include <pebble.h>
#include <string.h>
#include <stdlib.h>

#define NUM_MSGS 6

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

int hours_military_to_reg(int hour) {
    if (hour % 12 == 0) {
        return 12;
    }
    return hour % 12;
}

void gen_msg(char *buf, size_t buf_len, struct tm *tick_time) {
    int hour, min;
    hour = hours_military_to_reg(tick_time->tm_hour);
    min = tick_time->tm_min;

    const char *msgs[NUM_MSGS] = {
        "\nI don't know.",
        "\nDoes it matter?",
        "\nTime to get a watch.",
        "\nTime is an illusion. Lunchtime doubly so."
    };
    
    char tmp_buf[64] = {0};
    char my_buf[64] = {0};

    if (min >= 0 && min <= 10) {
        strncpy(tmp_buf, "\nIt's ", buf_len - 1); 
        strncat(tmp_buf, hour_int_to_str(hour), buf_len - 1);

        strncpy(my_buf, "\nIt's ", buf_len - 1);
        strncat(my_buf, hour_int_to_str(hour), buf_len - 1);
        strncat(my_buf, "-ish", buf_len - 1);
    }
    else if (min > 10 && min <= 20) {
        strncpy(tmp_buf, "\nIt's a quarter after.", buf_len - 1);

        if (min < 16) {
            strncpy(my_buf, "\nCheck your phone.", buf_len - 1);
        } else {
            strncpy(my_buf, "\nIt's ", buf_len - 1);
            strncat(my_buf, hour_int_to_str(hour), buf_len - 1);
            strncat(my_buf, ", like, eighteen?", buf_len - 1);
        }
    }
    else if (min >= 20 && min <= 27) {
        strncpy(tmp_buf, "\nIt's almost half past ", buf_len - 1);
        strncat(tmp_buf, hour_int_to_str(hour), buf_len - 1);

        if (min < 24) {
            strncpy(my_buf, "\nTwenty after.", buf_len - 1);
        } else {
            strncpy(my_buf, "It's ", buf_len - 1);
            strncat(my_buf, hour_int_to_str(hour), buf_len - 1);
            strncat(my_buf, " twenty-five. I think. Wait.", buf_len - 1);
        }
    }
    else if (min >= 28 && min <=32) {
        strncpy(tmp_buf, "\nIt's half past ", buf_len - 1);
        strncat(tmp_buf, hour_int_to_str(hour), buf_len - 1);
        
        strncpy(my_buf, "\nIt's ", buf_len - 1);
        strncat(my_buf, hour_int_to_str(hour), buf_len - 1);
        strncat(my_buf, " fifteen.", buf_len - 1);
    }
    else if (min >= 33 && min <= 39) {
        strncpy(tmp_buf, "\nIt's ", buf_len - 1);
        strncat(tmp_buf, hour_int_to_str(hour), buf_len - 1);
        strncat(tmp_buf, " thirty-something.", buf_len - 1);

        strncpy(my_buf, "\nIt's not quite ", buf_len - 1);
        strncat(my_buf, hour_int_to_str(hour), buf_len - 1);
        strncat(my_buf, " forty", buf_len - 1);
        if (min > 35) {
            strncat(my_buf, ", but my watch is fast.", buf_len - 1);
        } else {
            strncat(my_buf, ".", buf_len - 1);
        }

    }
    else if (min >= 40 && min <= 50) {
        strncpy(tmp_buf, "\nIt's a quarter to ", buf_len - 1);
        strncat(tmp_buf, hour_int_to_str(hour % 12 + 1), buf_len - 1);

        strncpy(my_buf, "\nIt's like, the big hand's on the nine? \
            It's near the nine.", buf_len -1);
    }
    else {
        strncpy(tmp_buf, "\nIt's almost ", buf_len - 1);
        strncat(tmp_buf, hour_int_to_str(hour % 12 + 1), buf_len - 1);

        strncpy(my_buf, "\nNearly ", buf_len - 1);
        strncat(my_buf, hour_int_to_str(hour % 12 + 1), buf_len - 1);
    }

    msgs[NUM_MSGS - 2] = &tmp_buf[0];
    msgs[NUM_MSGS - 1] = &my_buf[0];
    memset(buf, 0, buf_len);
    strncpy(buf, rand() % 2 ? msgs[NUM_MSGS - 2] : msgs[rand() % NUM_MSGS], buf_len);

    srand(time(NULL));
    


}

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    static char buf[64] = {0};
    strftime(buf, sizeof(buf), "\n%I:%M", tick_time);
    gen_msg(buf, sizeof(buf), tick_time);

    text_layer_set_text(text_layer, buf);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
    update_time();
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

#if defined(PBL_ROUND)
    text_layer_enable_screen_text_flow_and_paging(text_layer, 2);
#endif
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

    accel_tap_service_subscribe(tap_handler);
    update_time();
}

static void deinit() {
    window_destroy(window);

    accel_tap_service_unsubscribe();
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
