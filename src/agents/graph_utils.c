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

graph_context_t *duplicate_graph_context(graph_context_t *graph_context) {
    grid_t *         grid              = graph_context->grid;
    graph_context_t *new_graph_context = create_graph_context(grid);

    copy_graph_context(graph_context, new_graph_context);

    return new_graph_context;
}

void copy_graph_context(graph_context_t *source_graph_context, graph_context_t *dest_graph_context) {
    grid_t *grid             = source_graph_context->grid;
    dest_graph_context->grid = grid;

    // This could be a memcpy, but explicitly copying all data seems safer
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            dest_graph_context->path[x][y].next_direction = source_graph_context->path[x][y].next_direction;
        }
    }
}

void destroy_graph_context(graph_context_t *graph_context) {
    assert(graph_context != NULL);
    assert(graph_context->path != NULL);

    free(graph_context->path[0]);
    free(graph_context->path);
    free(graph_context);
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
    assert(x < graph_context->grid->width);
    assert(y < graph_context->grid->height);

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

uint16_t snake_distance_to_cherry(graph_context_t *graph_context) {
    grid_t *grid                = graph_context->grid;
    coord_t snake_head_position = {.x = grid->snake_head_x, .y = grid->snake_head_y};

    coord_t cherry_position;

    get_cherry_position(grid, &cherry_position.x, &cherry_position.y);

    return path_distance(graph_context, snake_head_position, cherry_position);
}

// FIXME: If we get stuck in a loop where target isn't reachable from source,
// we should signal it in some way. Currently we just return a really big value.
uint16_t path_distance(graph_context_t *graph_context, coord_t source, coord_t target) {
    assert(graph_context != NULL);
    assert(source.x <= graph_context->grid->width);
    assert(source.y <= graph_context->grid->height);
    assert(target.x <= graph_context->grid->width);
    assert(target.y <= graph_context->grid->height);

    uint8_t  x        = source.x;
    uint8_t  y        = source.y;
    uint8_t  x2       = source.x;
    uint8_t  y2       = source.y;
    uint16_t distance = 0;

    do {
        distance++;
        direction_t direction = graph_context->path[x][y].next_direction;

        switch (direction) {
            case RIGHT: x++; break;
            case LEFT: x--; break;
            case UP: y--; break;
            case DOWN: y++; break;
        }

        for (int i = 0; i < 2; i++) {
            direction = graph_context->path[x2][y2].next_direction;

            switch (direction) {
                case RIGHT: x2++; break;
                case LEFT: x2--; break;
                case UP: y2--; break;
                case DOWN: y2++; break;
            }
        }
    } while ((x != target.x || y != target.y) && (x != x2 || y != y2));

    if (x == x2 && y == y2)
        return (uint16_t)-1;

    return distance;
}

bool is_graph_fully_connected(graph_context_t *graph_context) {
    assert(graph_context != NULL);
    grid_t *grid = graph_context->grid;

    uint8_t  x               = 0;
    uint8_t  y               = 0;
    uint8_t  x2              = 0;
    uint8_t  y2              = 0;
    uint32_t visited         = 0;
    uint32_t number_of_cells = grid->width * grid->height;

    do {
        visited++;
        direction_t direction = graph_context->path[x][y].next_direction;

        switch (direction) {
            case RIGHT: x++; break;
            case LEFT: x--; break;
            case UP: y--; break;
            case DOWN: y++; break;
        }

        for (int i = 0; i < 2; i++) {
            direction = graph_context->path[x2][y2].next_direction;

            switch (direction) {
                case RIGHT: x2++; break;
                case LEFT: x2--; break;
                case UP: y2--; break;
                case DOWN: y2++; break;
            }
        }
    } while ((x != 0 || y != 0) && (x != x2 || y != y2));

    if (grid->width % 2 == 1 && grid->height % 2 == 1)
        return visited == number_of_cells - 1;

    return visited == number_of_cells;
}

void _reset_path_ids(graph_context_t *graph_context) {
    grid_t *grid = graph_context->grid;

    for (int y = 0; y < grid->height; y++)
        for (int x = 0; x < grid->width; x++)
            graph_context->path[x][y].path_id = -1;
}

void _tag_path(graph_context_t *graph_context, uint8_t tag_id, coord_t position) {
    uint8_t x = position.x;
    uint8_t y = position.y;

    assert(x < graph_context->grid->width);
    assert(y < graph_context->grid->height);

    /*print_path(graph_context);*/

    do {
        graph_context->path[x][y].path_id = tag_id;
        direction_t direction             = graph_context->path[x][y].next_direction;

        switch (direction) {
            case RIGHT: x++; break;
            case LEFT: x--; break;
            case UP: y--; break;
            case DOWN: y++; break;
        }
        /*printf("%d %d\n", x, y);*/

        assert(x < graph_context->grid->width);
        assert(y < graph_context->grid->height);
    } while (x != position.x || y != position.y);
}

uint8_t tag_paths(graph_context_t *graph_context) {
    uint8_t path_count = 0;

    // FIXME: This probably wont work with odd size grids

    grid_t *grid = graph_context->grid;

    _reset_path_ids(graph_context);
    /*printf("paths reset\n");*/

    while (true) {
        for (int y = 0; y < grid->height; y++) {
            for (int x = 0; x < grid->width; x++) {
                if (graph_context->path[x][y].path_id < 0) {
                    /*printf("found untagged at %d %d\n", x, y);*/
                    _tag_path(graph_context, path_count, (coord_t){.x = x, .y = y});
                    path_count++;
                    continue;
                }
            }
        }

        // Getting here means that we found no unvisited cells
        break;
    }

    return path_count;
}
