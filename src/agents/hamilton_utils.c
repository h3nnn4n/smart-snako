/*
 * Copyright <2021> <Renan S Silva, aka h3nnn4n>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <config.h>
#include <grid.h>
#include <utils.h>

#include "direction.h"
#include "graph_utils.h"
#include "hamilton_utils.h"

bool _build_halmiton_with_dfs(graph_context_t *graph_context, uint8_t x, uint8_t y);
void __print(graph_context_t *graph_context, uint8_t x_, uint8_t y_);

bool build_halmiton_with_dfs(graph_context_t *graph_context) {
    // :)
    return _build_halmiton_with_dfs(graph_context, 0, 0);
}

bool _build_halmiton_with_dfs(graph_context_t *graph_context, uint8_t x, uint8_t y) {
    grid_t *           grid            = graph_context->grid;
    static direction_t directions[4]   = {RIGHT, DOWN, UP, LEFT};
    static direction_t directions_2[4] = {LEFT, RIGHT, UP, DOWN};

    // The two left-most columns, whe the width is odd and we are not on the first row
    bool enable_odd_width_hack = (grid->width % 2 == 1) && (x < 2) && (y > 0);

    // When both the rows and columns are an odd number
    bool enable_odd_height_hack = (grid->height % 2 == 1) && (grid->width % 2 == 1);

    if (x == 0 && y == 0 && graph_context->path[x][y].visited) {
        if (get_config()->verbose) {
            printf("\n");
            print_path(graph_context);
            printf("\n");
        }

        if (all_cells_visited(graph_context)) {
            return true;
        }

        if (cells_not_visited_count(graph_context) == 1 && enable_odd_height_hack) {
            graph_context->path[1][2].hack = true;

            return true;
        }
    }

    if (graph_context->path[x][y].visited)
        return false;

    graph_context->path[x][y].visited = true;

    for (int i = 0; i < 4; i++) {
        direction_t direction = directions[i];

        if (enable_odd_width_hack)
            direction = directions_2[i];

        graph_context->path[x][y].next_direction = direction;

        uint8_t new_x = x;
        uint8_t new_y = y;

        switch (direction) {
            case RIGHT: new_x++; break;
            case LEFT: new_x--; break;
            case UP: new_y--; break;
            case DOWN: new_y++; break;
        }

        if (new_x >= grid->width || new_y >= grid->height)
            continue;

        // True means we found the target node (0, 0)
        if (_build_halmiton_with_dfs(graph_context, new_x, new_y))
            return true;
    }

    graph_context->path[x][y].visited = false;

    return false;
}

// The splice is applied to the square defined by x and y and x+1 and y+1
bool _apply_splice(graph_context_t *graph_context, coord_t position) {
    grid_t *grid = graph_context->grid;
    assert(position.x + 1 < grid->width);
    assert(position.y + 1 < grid->height);

    for (uint8_t x = 0; x <= 1; x++)
        for (uint8_t y = 0; y <= 1; y++)
            if (grid->cells[position.x + x][position.y + y].has_snake)
                return false;

    uint8_t x = position.x;
    uint8_t y = position.y;

    // Horizontal to vertical switch
    if (graph_context->path[x][y].next_direction == RIGHT) {
        if (graph_context->path[x + 1][y + 1].next_direction == LEFT) {
            graph_context->path[x][y].next_direction         = DOWN;
            graph_context->path[x + 1][y + 1].next_direction = UP;
            return true;
        }
    }

    if (graph_context->path[x + 1][y].next_direction == LEFT) {
        if (graph_context->path[x][y + 1].next_direction == RIGHT) {
            graph_context->path[x + 1][y].next_direction = DOWN;
            graph_context->path[x][y + 1].next_direction = UP;
            return true;
        }
    }

    // Vertical to horizontal switch
    if (graph_context->path[x][y].next_direction == DOWN) {
        if (graph_context->path[x + 1][y + 1].next_direction == UP) {
            graph_context->path[x][y].next_direction         = RIGHT;
            graph_context->path[x + 1][y + 1].next_direction = LEFT;
            return true;
        }
    }

    if (graph_context->path[x + 1][y].next_direction == DOWN) {
        if (graph_context->path[x][y + 1].next_direction == UP) {
            graph_context->path[x + 1][y].next_direction = LEFT;
            graph_context->path[x][y + 1].next_direction = RIGHT;
            return true;
        }
    }

    return false;
}

// This function takes a hamiltonian cycle, splices it in two, and then merge
// the path again at a different point. This generates a slightly different
// hamiltonian cycle. Can be used as a adjacency operator on a (meta) heuristic
bool perturbate_hamiltonian_cycle(graph_context_t *graph_context) {
    // FIXME needs some extra logic to handle odd grids

    // Can't work on a path not fully connected
    assert(tag_paths(graph_context) == 1);
    assert(is_graph_fully_connected(graph_context));

    grid_t *grid = graph_context->grid;

    uint8_t  first_splice_attempts  = 10;
    uint16_t second_splice_attempts = 50;

    uint8_t first_splice_x = 0;
    uint8_t first_splice_y = 0;

    bool splice_result;

    printf("\n");
    printf("-------------------------\n");
    printf("\n");
    /*print_path(graph_context);*/
    printf("super before\n");

    do {
        uint8_t x = get_random_number(grid->width - 1);
        uint8_t y = get_random_number(grid->height - 1);
        /*printf("%d %d\n", x, y);*/
        if (_apply_splice(graph_context, (coord_t){.x = x, .y = y})) {
            first_splice_x = x;
            first_splice_y = y;
            // This ensures that the splice is reversible
            splice_result = _apply_splice(graph_context, (coord_t){.x = x, .y = y});
            assert(splice_result);
            __print(graph_context, first_splice_x, first_splice_y);
            splice_result = _apply_splice(graph_context, (coord_t){.x = x, .y = y});
            assert(splice_result);
            break;
        }
    } while (--first_splice_attempts > 0);

    if (first_splice_attempts == 0)
        return false;

    assert(tag_paths(graph_context) == 2);
    assert(!is_graph_fully_connected(graph_context));

    printf("after first splice:\n");
    __print(graph_context, first_splice_x, first_splice_y);

    /*printf("yay first splice: %d\n", 10 - first_splice_attempts);*/
    /*uint8_t path_count = tag_paths(graph_context);*/
    /*printf("yay paths tagged: %d\n", path_count);*/

    do {
        uint8_t x = get_random_number(grid->width - 1);
        uint8_t y = get_random_number(grid->height - 1);
        /*printf("%d %d\n", x, y);*/
        if (_apply_splice(graph_context, (coord_t){.x = x, .y = y})) {
            if (is_graph_fully_connected(graph_context)) {
                break;
            } else {
                splice_result = _apply_splice(graph_context, (coord_t){.x = x, .y = y});
                assert(splice_result);
            }
        }
    } while (--second_splice_attempts > 0);

    /*printf("yay second splice: %d\n", 50 - second_splice_attempts);*/
    /*path_count = tag_paths(graph_context);*/
    /*printf("yay paths tagged: %d\n", path_count);*/
    printf("after second splice:\n");
    __print(graph_context, first_splice_x, first_splice_y);

    if (second_splice_attempts == 0) {
        // This undoes the first splice
        /*printf("\n");*/
        /*print_path(graph_context);*/
        /*path_count = tag_paths(graph_context);*/
        /*printf("??: %d\n", path_count);*/
        /*printf("%d %d\n", first_splice_x, first_splice_y);*/
        splice_result = _apply_splice(graph_context, (coord_t){.x = first_splice_x, .y = first_splice_y});
        assert(splice_result);
        printf("\n");
        printf("after\n");
        __print(graph_context, first_splice_x, first_splice_y);
        /*printf("\n");*/
        /*print_path(graph_context);*/
        /*path_count = tag_paths(graph_context);*/
        /*printf("??: %d\n", path_count);*/

        // Ensure everything is back in place
        assert(tag_paths(graph_context) == 1);
        assert(is_graph_fully_connected(graph_context));
        return false;
    }

    // Make linters happy when values are not used in assertions
    (void *)splice_result;

    // Ensure everything is back in place
    assert(tag_paths(graph_context) == 1);
    assert(is_graph_fully_connected(graph_context));
    return true;
}

void __print(graph_context_t *graph_context, uint8_t x_, uint8_t y_) {
    printf("\n");
    for (int y = -2; y <= 3; y++) {
        if ((uint8_t)(y + y_) >= graph_context->grid->height)
            continue;

        for (int x = -2; x <= 3; x++) {
            if ((uint8_t)(x + x_) >= graph_context->grid->width)
                continue;

            switch (graph_context->path[x_ + x][y_ + y].next_direction) {
                case RIGHT: printf("> "); break;
                case LEFT: printf("< "); break;
                case UP: printf("^ "); break;
                case DOWN: printf("V "); break;
                default:
                    printf("wtf\n");
                    abort();
                    break;
            }
        }
        printf("\n");
    }
}
