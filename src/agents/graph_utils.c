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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcg_variants.h>

#include <cherry.h>
#include <config.h>
#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "graph_utils.h"

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

void reset_graph_context(graph_context_t *graph_context) {
    grid_t * grid = graph_context->grid;
    tuple_t *path = graph_context->path[0];
    memset(path, 0, sizeof(tuple_t) * grid->width * grid->height);
}

uint32_t cells_not_visited_count(graph_context_t *graph_context) {
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

bool all_cells_visited(graph_context_t *graph_context) {
    grid_t *grid = graph_context->grid;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (!graph_context->path[x][y].visited)
                return false;
        }
    }

    return true;
}

void shuffle_directions(direction_t *directions, uint8_t n) {
    for (size_t i = n - 1; i > 0; i--) {
        size_t j      = pcg32_boundedrand(i + 1);
        int    t      = directions[j];
        directions[j] = directions[i];
        directions[i] = t;
    }
}

void build_reverse_path(graph_context_t *graph_context) {
    grid_t *grid     = graph_context->grid;
    uint8_t target_x = 0;
    uint8_t target_y = 0;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (graph_context->path[x][y].target) {
                target_x = x;
                target_y = y;
                /*printf("found target\n");*/
            }
        }
    }

    uint8_t x = target_x;
    uint8_t y = target_y;

    do {
        direction_t direction = graph_context->path[x][y].prev_direction;

        // Note that we are going backwards here. For target to source
        switch (direction) {
            case RIGHT: x--; break;
            case LEFT: x++; break;
            case UP: y++; break;
            case DOWN: y--; break;
        }

        graph_context->path[x][y].next_direction = direction;
    } while (graph_context->path[x][y].source != true);

    /*printf("found reverse path\n");*/
}

void set_graph_target(graph_context_t *graph_context, uint8_t x, uint8_t y) {
    assert(graph_context != NULL);
    assert(graph_context->grid != NULL);
    assert(graph_context->path != NULL);

    grid_t *grid = graph_context->grid;

    assert(x < grid->width);
    assert(y < grid->height);

    graph_context->path[x][y].target = true;
}

void print_path(graph_context_t *graph_context) {
    grid_t *grid = graph_context->grid;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (graph_context->path[x][y].visited) {
                switch (graph_context->path[x][y].next_direction) {
                    case RIGHT: printf("> "); break;
                    case LEFT: printf("< "); break;
                    case UP: printf("^ "); break;
                    case DOWN: printf("V "); break;
                }
            } else if (graph_context->path[x][y].target) {
                printf("O ");
            } else if (graph_context->path[x][y].blocked) {
                printf("@ ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void print_reverse_path(graph_context_t *graph_context) {
    grid_t *grid = graph_context->grid;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (graph_context->path[x][y].visited) {
                switch (graph_context->path[x][y].prev_direction) {
                    case RIGHT: printf("> "); break;
                    case LEFT: printf("< "); break;
                    case UP: printf("^ "); break;
                    case DOWN: printf("V "); break;
                    default: printf("invalid direction\n"); abort();
                }
            } else if (graph_context->path[x][y].target) {
                printf("O ");
            } else if (graph_context->path[x][y].blocked) {
                printf("@ ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void occupy_cells_with_snake(graph_context_t *graph_context) {
    grid_t *grid = graph_context->grid;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->cells[x][y].has_snake) {
                graph_context->path[x][y].blocked = true;
            }
        }
    }
}

uint32_t path_distance(graph_context_t *graph_context, coord_t source, coord_t target) {
    grid_t *grid = graph_context->grid;
    assert(graph_context != NULL);

    assert(source.x <= grid->width);
    assert(source.y <= grid->height);
    assert(target.x <= grid->width);
    assert(target.y <= grid->height);

    uint8_t  x        = source.x;
    uint8_t  y        = source.y;
    uint32_t distance = 0;

    // TODO(@h3nnn4n): Implement loop detection
    do {
        distance++;
        direction_t direction = graph_context->path[x][y].next_direction;

        switch (direction) {
            case RIGHT: x++; break;
            case LEFT: x--; break;
            case UP: y--; break;
            case DOWN: y++; break;
        }
    } while (x != target.x || y != target.y);

    return distance;
}
