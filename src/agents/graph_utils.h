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

#ifndef SRC_AGENTS_AGENT_UTILS_H_
#define SRC_AGENTS_AGENT_UTILS_H_

#include <stdbool.h>

#include <grid.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} coord_t;

typedef struct {
    // Where this node leads to
    direction_t next_direction;
    // What direction led to this node
    direction_t prev_direction;

    bool visited;
    bool hack;
    bool target;
    bool source;
    bool blocked;
} tuple_t;

typedef struct {
    tuple_t **path;
    grid_t *  grid;
} graph_context_t;

direction_t get_safe_random_direction(grid_t *grid);

graph_context_t *create_graph_context(grid_t *grid);
void             destroy_graph_context(graph_context_t *graph);
void             reset_graph_context(graph_context_t *graph_context);

void     occupy_cells_with_snake(graph_context_t *graph_context);
void     set_graph_target(graph_context_t *graph_context, uint8_t x, uint8_t y);
uint32_t cells_not_visited_count(graph_context_t *graph);
bool     all_cells_visited(graph_context_t *graph);

void build_reverse_path(graph_context_t *graph_context);

void print_path(graph_context_t *graph_context);
void print_reverse_path(graph_context_t *graph_context);

void shuffle_directions(direction_t *directions, uint8_t n);

uint32_t path_distance(graph_context_t *graph_context, coord_t source, coord_t target);

#endif  // SRC_AGENTS_AGENT_UTILS_H_
