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

#include <stdbool.h>

#include "cherry.h"
#include "grid.h"
#include "utils.h"

// TODO(@h3nnn4n): We could cache this
bool has_cherry(grid_t *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->cells[x][y].has_cherry) {
                return true;
            }
        }
    }

    return false;
}

// TODO(@h3nnn4n): This could be smarter
void spawn_cherry(grid_t *grid) {
    while (true) {
        uint8_t x = get_random_number(grid->width);
        uint8_t y = get_random_number(grid->height);

        if (grid->cells[x][y].has_snake)
            continue;

        if (grid->cells[x][y].has_cherry)
            continue;

        grid->cells[x][y].has_cherry = true;
        break;
    }
}

bool eat_cherry(grid_t *grid, uint8_t x, uint8_t y) {
    if (!grid->cells[x][y].has_cherry)
        return false;

    grid->cells[x][y].has_cherry = false;

    register_cherry_eaten(grid->stats);

    return true;
}
