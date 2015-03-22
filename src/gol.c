#include "pebble.h"

#define TICK 300
#define BOARD_WIDTH 50
#define BOARD_HEIGHT 50

static Window* window;
int board[BOARD_WIDTH * BOARD_HEIGHT];
int temp_board[BOARD_WIDTH * BOARD_HEIGHT];

/*
 * GAME LOGIC
 */

static int board_get_cell(int* current_board, int x, int y) {
    if (x >= BOARD_WIDTH || x < 0 || y >= BOARD_HEIGHT || y < 0) {
        return 0;
    }
    return current_board[y * BOARD_WIDTH + x];
}

static int count_neighbors(int* current_board, int x, int y) {
    int result = 0;

    if (board_get_cell(current_board, x - 1, y - 1) == 1) result++;
    if (board_get_cell(current_board, x, y - 1) == 1) result++;
    if (board_get_cell(current_board, x + 1, y - 1) == 1) result++;
    if (board_get_cell(current_board, x - 1, y) == 1) result++;
    if (board_get_cell(current_board, x + 1, y) == 1) result++;
    if (board_get_cell(current_board, x - 1, y + 1) == 1) result++;
    if (board_get_cell(current_board, x, y + 1) == 1) result++;
    if (board_get_cell(current_board, x + 1, y + 1) == 1) result++;
    return result;
}

static void update_board() {
    int y, x, neighbors, current;

    for (y = 0; y < BOARD_HEIGHT; y++) {
        for (x = 0; x < BOARD_WIDTH; x++) {
            neighbors = count_neighbors(board, x, y);
            current = board_get_cell(board, x, y);
            if (current == 1) {
                if (neighbors == 2 || neighbors == 3) {
                    temp_board[y * BOARD_WIDTH + x] = 1;
                } else {
                    temp_board[y * BOARD_WIDTH + x] = 0;
                }
            } else {
                if (neighbors == 3) {
                    temp_board[y * BOARD_WIDTH + x] = 1;
                } else {
                    temp_board[y * BOARD_WIDTH + x] = 0;
                }
            }
        }
    }
    // copy temp_board to main board
    for (y = 0; y < BOARD_HEIGHT; y++) {
        for (x = 0; x < BOARD_WIDTH; x++) {
            board[y * BOARD_WIDTH + x] = temp_board[y * BOARD_WIDTH + x];
        }
    }

}


static void setup_board() {
    int y, x;

    for (y = 0; y < BOARD_HEIGHT; y++) {
        for (x = 0; x < BOARD_WIDTH; x++) {
            board[y * BOARD_WIDTH + x] = 0;
            temp_board[y * BOARD_WIDTH + x] = 0;
        }
    }

    // FIXME: hardcoded r pentomino for now
    board[30 + 30 * BOARD_WIDTH] = 1;
    board[31 + 30 * BOARD_WIDTH] = 1;
    board[30 + 31 * BOARD_WIDTH] = 1;
    board[29 + 31 * BOARD_WIDTH] = 1;
    board[30 + 32 * BOARD_WIDTH] = 1;

}

// Graphics routines
static void draw_cell(GContext* ctx, GRect bounds, int x, int y) {
    graphics_fill_circle(ctx, GPoint(x * 2, y * 2), 1);
}

static void draw_board(GContext* ctx, GRect bounds) {
    int y, x;

    graphics_context_set_fill_color(ctx, GColorWhite);
    for (y = 0; y < BOARD_HEIGHT; y++) {
        for (x = 0; x < BOARD_WIDTH; x++) {
            if (board[y * BOARD_WIDTH + x] == 1) {
                draw_cell(ctx, bounds, x, y);
            }
        }
    }
}

static void window_layer_draw(Layer* layer, GContext* ctx) {
    GRect bounds = layer_get_bounds(layer);

    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);

    draw_board(ctx, bounds);
    update_board();
}

/*
 * Boilerplate: Loop, init and deinit.
 */

static void loop_update(void* data) {
    Layer* window_layer = window_get_root_layer(window);
    layer_mark_dirty(window_layer);
    app_timer_register(TICK, loop_update, NULL);
}

static void window_load(Window* window) {
    Layer *window_layer = window_get_root_layer(window);
    layer_set_update_proc(window_layer, window_layer_draw);
}

static void window_unload(Window* window) {
    // no-op
}

static void init(void) {
    // Set up the window
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, true); //Yes animated

    // Start a timer to run game updates
    app_timer_register(TICK, loop_update, NULL);
    setup_board();
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
