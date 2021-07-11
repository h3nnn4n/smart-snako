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

#ifndef SRC_AGENTS_UTILS_H_
#define SRC_AGENTS_UTILS_H_

#include <stdbool.h>

#include <grid.h>

typedef struct {
    uint8_t x;
    uint8_t y;

    bool visited;

    direction_t next_direction;

    bool hack;
} tuple_t;

typedef struct {
    tuple_t **path;
    grid_t *  grid;
} graph_context_t;

graph_context_t *create_graph_context(grid_t *grid);
void             destroy_graph_context(graph_context_t *graph);

direction_t get_safe_random_direction(grid_t *grid);

uint32_t _cells_not_visited_count(graph_context_t *graph);
bool     _all_cells_visited(graph_context_t *graph);

#endif  // SRC_AGENTS_UTILS_H_
