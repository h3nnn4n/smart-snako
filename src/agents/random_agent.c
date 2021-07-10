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
#include <utils.h>

#include "random_agent.h"

static direction_t last_direction;

direction_t random_agent(grid_t *t) {
    direction_t new_direction = LEFT;

    do {
        new_direction = get_random_direction();

        if (new_direction == RIGHT && last_direction == LEFT) continue;
        if (new_direction == LEFT && last_direction == RIGHT) continue;

        if (new_direction == UP && last_direction == DOWN) continue;
        if (new_direction == DOWN && last_direction == UP) continue;

        break;
    } while (true);

    last_direction = new_direction;

    return new_direction;
}
