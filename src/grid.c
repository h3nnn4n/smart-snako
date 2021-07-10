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

#include "cherry.h"
#include "config.h"
#include "grid.h"
#include "snake.h"
#include "stats.h"

void snake_init(grid_t *grid) {
    int counter = 0;
    int y       = 1;

    for (int x = 2; x < 5; x++) {
        counter++;
        grid->cells[x][y].has_snake     = true;
        grid->cells[x][y].snake_counter = counter;

        if (counter > 0)
            grid->cells[x][y].previous_snake_cell = &grid->cells[x - 1][y];
    }

    grid->snake_head_x = 4;
    grid->snake_head_y = 1;
}

grid_t *create_grid(uint8_t width, uint8_t height) {
    assert(width > 0);
    assert(height > 0);

    grid_t *grid = malloc(sizeof(grid_t));
    memset(grid, 0, sizeof(grid_t));

    grid->width  = width;
    grid->height = height;

    grid->max_moves_without_cherry = grid->width * grid->height;

    cell_t *cells = malloc(sizeof(cell_t) * grid->width * grid->height);
    memset(cells, 0, sizeof(cell_t) * grid->width * grid->height);

    grid->cells = malloc(sizeof(cell_t *) * grid->width);
    memset(grid->cells, 0, sizeof(cell_t *) * grid->width);

    for (uint8_t i = 0; i < grid->width; i++) {
        grid->cells[i] = &cells[i * grid->height];
    }

    snake_init(grid);

    grid->stats       = create_stats();
    grid->stats->grid = grid;

    return grid;
}

void destroy_grid(grid_t *grid) {
    destroy_stats(grid->stats);
    free(grid->cells[0]);
    free(grid->cells);
    free(grid);
}

void simulate(grid_t *grid, direction_t direction) {
    if (!has_cherry(grid))
        spawn_cherry(grid);

    if (is_snake_colliding(grid, direction)) {
        printf("snake collided\n");
        set_game_over(grid);
        return;
    }

    if (grid->stats->moves_since_last_cherry > grid->max_moves_without_cherry) {
        printf("snake died of hunger\n");
        set_game_over(grid);
        return;
    }

    move_snake(grid, direction);
}

void print_grid(grid_t *grid) {
    if (!get_config()->verbose)
        return;

    printf("\n");
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->cells[x][y].has_cherry) {
                printf("O ");
            } else if (grid->cells[x][y].has_snake) {
                if (x == grid->snake_head_x && y == grid->snake_head_y) {
                    printf("C ");
                } else {
                    printf("x ");
                }
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void set_game_over(grid_t *grid) { grid->game_over = true; }

bool is_game_over(grid_t *grid) { return grid->game_over; }
