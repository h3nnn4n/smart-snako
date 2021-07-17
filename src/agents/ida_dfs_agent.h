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

#ifndef SRC_AGENTS_IDA_DFS_AGENT_H_
#define SRC_AGENTS_IDA_DFS_AGENT_H_

#include <grid.h>
#include <utils.h>

#include "graph_utils.h"

typedef struct {
    bool    path_found;
    uint8_t cherry_x;
    uint8_t cherry_y;

    graph_context_t *graph_context;
} ida_dfs_agent_context_t;

void        ida_dfs_agent_create(grid_t *grid);
void        ida_dfs_agent_destroy(grid_t *grid);
direction_t ida_dfs_agent(grid_t *grid);

#endif  // SRC_AGENTS_IDA_DFS_AGENT_H_
