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

#include "grid.h"

void snake_init(grid_t *grid) {
    int counter = 0;
    int y       = 1;

    for (int x = 2; x < 5; x++) {
        counter++;
        grid->cells[x][y].has_snake     = true;
        grid->cells[x][y].snake_counter = counter;
    }
}

grid_t *create_grid(uint8_t width, uint8_t height) {
    assert(width > 0);
    assert(height > 0);

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

    snake_init(grid);

    return grid;
}

void destroy_grid(grid_t *grid) {
    free(grid->cells[0]);
    free(grid->cells);
    free(grid);
}

void print_grid(grid_t *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->cells[x][y].has_cherry) {
                printf("O ");
            } else if (grid->cells[x][y].has_snake) {
                printf("X ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}
