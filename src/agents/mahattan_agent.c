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
#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "agent_utils.h"
#include "mahattan_agent.h"

void mahattan_agent_create(grid_t *grid) {
    grid->agent_context                   = malloc(sizeof(direction_t));
    *(direction_t *)(grid->agent_context) = RIGHT;
}

void mahattan_agent_destroy(grid_t *grid) {
    assert(grid->agent_context != NULL);
    free(grid->agent_context);
}

direction_t mahattan_agent(grid_t *grid) {
    direction_t *last_direction = (direction_t *)grid->agent_context;
    direction_t  new_direction  = get_safe_random_direction(grid, *last_direction);

    uint8_t cherry_x, cherry_y;
    uint8_t snake_x = grid->snake_head_x;
    uint8_t snake_y = grid->snake_head_y;

    if (!get_cherry_position(grid, &cherry_x, &cherry_y)) {
        printf("WARN: found no cherry\n");
        return new_direction;
    }

    if (snake_x > cherry_x && !is_snake_colliding(grid, LEFT)) {
        new_direction = LEFT;
    }

    if (snake_x < cherry_x && !is_snake_colliding(grid, RIGHT)) {
        new_direction = RIGHT;
    }

    if (snake_y > cherry_y && !is_snake_colliding(grid, UP)) {
        new_direction = UP;
    }

    if (snake_y < cherry_y && !is_snake_colliding(grid, DOWN)) {
        new_direction = DOWN;
    }

    *last_direction = new_direction;

    return new_direction;
}
