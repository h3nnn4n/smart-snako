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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <cherry.h>
#include <config.h>
#include <grid.h>

#include "graph_utils.h"

bool _dfs(graph_context_t *graph_context, uint8_t x, uint8_t y, int32_t max_depth);

bool dfs(graph_context_t *graph_context, uint8_t x, uint8_t y) {
    grid_t *grid = graph_context->grid;
    uint8_t cherry_x, cherry_y;
    get_cherry_position(grid, &cherry_x, &cherry_y);

    reset_graph_context(graph_context);
    set_graph_target(graph_context, cherry_x, cherry_y);
    occupy_cells_with_snake(graph_context);
    graph_context->path[x][y].source = true;

    /*printf("aa\n");*/
    /*print_grid(graph_context->grid);*/
    /*printf("bb\n");*/
    /*print_path(graph_context);*/
    /*printf("cc\n");*/

    bool result = _dfs(graph_context, x, y, -1);

    if (result) {
        build_reverse_path(graph_context);

        if (get_config()->verbose) {
            printf("\n");
            print_path(graph_context);
            printf("\n");
        }
    }

    return result;
}

bool ida_dfs(graph_context_t *graph_context, uint8_t x, uint8_t y) {
    grid_t *grid = graph_context->grid;
    uint8_t cherry_x, cherry_y;
    get_cherry_position(grid, &cherry_x, &cherry_y);

    // If the cherry is 10 units away, running IDA with a depth less than that
    // wont ever find a path. So we start on a reasonably close value
    uint32_t estimated_distance_to_cherry   = abs(x - cherry_x) + abs(y - cherry_y);
    uint32_t depth                          = estimated_distance_to_cherry - 2;
    uint32_t current_cell_not_visited_count = 0;
    uint8_t  fail_count                     = 0;

    do {
        depth++;
        reset_graph_context(graph_context);
        set_graph_target(graph_context, cherry_x, cherry_y);
        occupy_cells_with_snake(graph_context);
        graph_context->path[x][y].source = true;

        if (_dfs(graph_context, x, y, depth)) {
            build_reverse_path(graph_context);

            if (get_config()->verbose) {
                printf("\n");
                print_path(graph_context);
                printf("\n");
            }

            return true;
        }

        uint32_t last_cell_not_visited_count = current_cell_not_visited_count;
        current_cell_not_visited_count       = cells_not_visited_count(graph_context);

        if (last_cell_not_visited_count == current_cell_not_visited_count)
            fail_count++;
        else
            fail_count = 0;
    } while (fail_count < 2);

    return false;
}

bool _dfs(graph_context_t *graph_context, uint8_t x, uint8_t y, int32_t max_depth) {
    /*printf("%d %d %d\n", x, y, max_depth);*/
    grid_t *    grid          = graph_context->grid;
    direction_t directions[4] = {LEFT, RIGHT, DOWN, UP};
    shuffle_directions(directions, 4);

    /*printf("\n");*/
    /*print_reverse_path(graph_context);*/

    if (graph_context->path[x][y].target) {
        /*printf("found target at %u %u depth: %d\n", x, y, max_depth);*/
        return true;
    }

    if (max_depth == 0)
        return false;

    if (graph_context->path[x][y].visited)
        return false;

    graph_context->path[x][y].visited = true;

    // We should skip any blocked nodes, except if it is the snake's head,
    // because that is where the dfs usually starts at
    if (graph_context->path[x][y].blocked)
        if (x != grid->snake_head_x || y != grid->snake_head_y)
            return false;

    for (int i = 0; i < 4; i++) {
        direction_t direction = directions[i];

        uint8_t new_x = x;
        uint8_t new_y = y;

        switch (direction) {
            case RIGHT: new_x++; break;
            case LEFT: new_x--; break;
            case UP: new_y--; break;
            case DOWN: new_y++; break;
            default: printf("ERROR: Got invalid direction\n"); abort();
        }

        if (new_x >= grid->width || new_y >= grid->height)
            continue;

        if (graph_context->path[new_x][new_y].visited)
            continue;

        if (graph_context->path[new_x][new_y].blocked)
            continue;

        graph_context->path[new_x][new_y].prev_direction = direction;

        // True means we found the target node
        if (_dfs(graph_context, new_x, new_y, max_depth - 1))
            return true;
    }

    /*graph_context->path[x][y].visited = false;*/

    /*printf("\n");*/
    /*print_reverse_path(graph_context);*/

    return false;
}
