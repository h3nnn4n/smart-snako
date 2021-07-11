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
#include <stdlib.h>
#include <string.h>

#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "agent_utils.h"

direction_t get_safe_random_direction(grid_t *grid) {
    direction_t new_direction;

    uint8_t max_tries = 50;

    do {
        max_tries--;
        new_direction = get_random_direction();

        if (is_snake_colliding(grid, new_direction))
            continue;
        else
            return new_direction;

        break;
    } while (max_tries > 0);

    return RIGHT;
}

graph_context_t *create_graph_context(grid_t *grid) {
    graph_context_t *graph = malloc(sizeof(graph_context_t));
    memset(graph, 0, sizeof(graph_context_t));

    graph->grid = grid;

    tuple_t *path = malloc(sizeof(tuple_t) * grid->width * grid->height);
    memset(path, 0, sizeof(tuple_t) * grid->width * grid->height);

    graph->path = malloc(sizeof(tuple_t *) * grid->width);
    memset(graph->path, 0, sizeof(tuple_t *) * grid->width);
    graph->path[0] = path;

    for (uint8_t i = 0; i < grid->width; i++) {
        graph->path[i] = &path[i * grid->height];
    }

    return graph;
}

void destroy_graph_context(graph_context_t *graph) {
    assert(graph != NULL);
    free(graph);
}

uint32_t _cells_not_visited_count(graph_context_t *graph_context) {
    grid_t * grid  = graph_context->grid;
    uint32_t count = 0;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (!graph_context->path[x][y].visited)
                count++;
        }
    }

    return count;
}

bool _all_cells_visited(graph_context_t *graph_context) {
    grid_t *grid = graph_context->grid;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (!graph_context->path[x][y].visited)
                return false;
        }
    }

    return true;
}
