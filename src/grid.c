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

#include <stdlib.h>
#include <string.h>

#include "grid.h"

grid_t *allocate_grid(uint8_t width, uint8_t height) {
    grid_t *grid = malloc(sizeof(grid_t));
    memset(grid, 0, sizeof(grid_t));

    grid->width  = width;
    grid->height = height;

    cell_t *cells = malloc(sizeof(cell_t) * grid->width * grid->height);
    memset(cells, 0, sizeof(cell_t) * grid->width * grid->height);

    grid->cells = malloc(sizeof(cell_t *) * grid->width);
    for (uint8_t i = 0; i < grid->width; i++) {
        grid->cells[i] = &cells[i * grid->height];
    }

    return grid;
}
