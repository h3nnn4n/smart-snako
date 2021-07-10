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
#include <stdlib.h>

#include "cherry.h"
#include "grid.h"
#include "snake.h"

bool is_snake_colliding(grid_t *grid, direction_t direction) {
    uint8_t new_head_x = grid->snake_head_x;
    uint8_t new_head_y = grid->snake_head_y;

    switch (direction) {
        case RIGHT: new_head_x++; break;
        case LEFT: new_head_x--; break;
        case UP: new_head_y--; break;
        case DOWN: new_head_y++; break;
    }

    // No need to check for lower bounds because var type is unsigned If it
    // tries to go below zero it underflows and becomes bigger than the size
    // limit
    if (new_head_x >= grid->width)
        return true;

    if (new_head_y >= grid->height)
        return true;

    if (grid->cells[new_head_x][new_head_y].snake_counter > 0)
        return true;

    return false;
}

void move_snake(grid_t *grid, direction_t direction) {
    uint8_t new_head_x = grid->snake_head_x;
    uint8_t new_head_y = grid->snake_head_y;

    switch (direction) {
        case RIGHT: new_head_x++; break;
        case LEFT: new_head_x--; break;
        case UP: new_head_y--; break;
        case DOWN: new_head_y++; break;
    }

    eat_cherry(grid, new_head_x, new_head_y);

    grid->cells[new_head_x][new_head_y].has_snake           = true;
    grid->cells[new_head_x][new_head_y].previous_snake_cell = &grid->cells[grid->snake_head_x][grid->snake_head_y];
    grid->cells[new_head_x][new_head_y].snake_counter =
        grid->cells[grid->snake_head_x][grid->snake_head_y].snake_counter;

    cell_t *cell = &grid->cells[new_head_x][new_head_y];

    do {
        cell = cell->previous_snake_cell;
        cell->snake_counter--;

        if (cell->snake_counter == 1) {
            cell->previous_snake_cell->snake_counter = 0;
            cell->previous_snake_cell->has_snake     = false;
            cell->previous_snake_cell                = NULL;
            cell                                     = NULL;
            break;
        }
    } while (cell != NULL);

    grid->snake_head_x = new_head_x;
    grid->snake_head_y = new_head_y;
}
