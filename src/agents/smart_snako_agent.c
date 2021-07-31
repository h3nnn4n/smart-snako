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

#include <cherry.h>
#include <config.h>
#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "dfs.h"
#include "graph_utils.h"
#include "hamilton_utils.h"
#include "smart_snako_agent.h"

void smart_snako_agent_create(grid_t *grid) {
    grid->agent_context = malloc(sizeof(smart_snako_agent_context_t));
    memset(grid->agent_context, 0, sizeof(smart_snako_agent_context_t));

    smart_snako_agent_context_t *agent_context = (smart_snako_agent_context_t *)grid->agent_context;

    agent_context->graph_context  = create_graph_context(grid);
    agent_context->graph_context_ = create_graph_context(grid);

    bool result = build_halmiton_with_dfs(agent_context->graph_context);
    if (!result)
        printf("WARN: failed to build halmitonian cycle\n");

    copy_graph_context(agent_context->graph_context, agent_context->graph_context_);
}

void smart_snako_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);

    smart_snako_agent_context_t *agent_context = (smart_snako_agent_context_t *)grid->agent_context;

    destroy_graph_context(agent_context->graph_context);
    destroy_graph_context(agent_context->graph_context_);

    free(grid->agent_context);
}

void try_reducing_distance_to_cherry(smart_snako_agent_context_t *context) {
    graph_context_t *graph_context = context->graph_context;
    grid_t *         grid          = graph_context->grid;

    // When the snake fills the grid there won't be cherries anymore
    if (!has_cherry(grid))
        return;

    for (uint8_t i = 0; i < SMART_SNAKO_PERTURBATE_PATH_ATTEMPTS; i++) {
        uint16_t original_distance   = snake_distance_to_cherry(graph_context);
        bool     perturbation_result = perturbate_hamiltonian_cycle(graph_context);
        uint16_t new_distance        = snake_distance_to_cherry(graph_context);

        if (perturbation_result) {
            if (new_distance >= original_distance) {
                copy_graph_context(context->graph_context_, context->graph_context);
            } else {
                copy_graph_context(context->graph_context, context->graph_context_);
            }
        }
    }
}

direction_t smart_snako_agent(grid_t *grid) {
    smart_snako_agent_context_t *context       = (smart_snako_agent_context_t *)grid->agent_context;
    graph_context_t *            graph_context = context->graph_context;

    uint8_t x = grid->snake_head_x;
    uint8_t y = grid->snake_head_y;

    try_reducing_distance_to_cherry(context);

    direction_t next_direction = graph_context->path[x][y].next_direction;

    if (is_snake_colliding(grid, next_direction))
        return get_safe_random_direction(grid);

    if (graph_context->path[x][y].hack) {
        uint8_t cherry_x, cherry_y;
        get_cherry_position(grid, &cherry_x, &cherry_y);

        if (x == cherry_y && y == cherry_y + 1)
            return UP;
    }

    return next_direction;
}
