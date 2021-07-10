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

#include <grid.h>
#include <snake.h>
#include <utils.h>

#include "random_agent.h"

static direction_t last_direction;

// TODO(@h3nnn4n): This is very dumb and wasteful of cpu time, but since this
// is supposed to be a baseline agent it is fine for now.
direction_t random_agent(grid_t *grid) {
    direction_t new_direction = LEFT;

    uint8_t max_tries = 50;

    do {
        max_tries--;
        new_direction = get_random_direction();

        if (new_direction == RIGHT && last_direction == LEFT)
            continue;
        if (new_direction == LEFT && last_direction == RIGHT)
            continue;

        if (new_direction == UP && last_direction == DOWN)
            continue;
        if (new_direction == DOWN && last_direction == UP)
            continue;

        if (is_snake_colliding(grid, new_direction))
            continue;

        break;
    } while (max_tries > 0);

    last_direction = new_direction;

    return new_direction;
}
