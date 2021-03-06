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

#ifndef SRC_GRID_H_
#define SRC_GRID_H_

#include <stdint.h>

#include "cell.h"
#include "direction.h"
#include "stats.h"

typedef struct grid_s {
    bool game_over;

    uint8_t  width;
    uint8_t  height;
    uint16_t max_moves_without_cherry;

    uint8_t snake_head_x;
    uint8_t snake_head_y;

    cell_t **cells;

    void *agent_context;

    struct stats_s *stats;
} grid_t;

grid_t *create_grid(uint8_t width, uint8_t height);
void    destroy_grid(grid_t *grid);
void    print_grid(grid_t *grid);
void    simulate(grid_t *grid, direction_t direction);
void    set_game_over(grid_t *grid);
bool    is_game_over(grid_t *grid);

#endif  // SRC_GRID_H_
