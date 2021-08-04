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

#ifndef SRC_STATS_H_
#define SRC_STATS_H_

#include <stdint.h>
#include <time.h>

#include "grid.h"

typedef struct stats_s {
    char *   agent_name;
    uint32_t cherries_eaten;
    uint32_t total_moves;
    uint32_t moves_since_last_cherry;
    double   agent_runtime;

    struct timespec runtime_start;
    struct timespec runtime_end;

    struct grid_s *grid;
} stats_t;

stats_t *create_stats();
void     destroy_stats(stats_t *stats);
void     register_move(stats_t *stats);
void     register_cherry_eaten(stats_t *stats);
void     register_agent_runtime_start(stats_t *stats);
void     register_agent_runtime_end(stats_t *stats);
void     print_stats(stats_t *stats);
void     dump_stats(stats_t *stats);
void     set_agent_name(stats_t *stats, char *agent_name);

#endif  // SRC_STATS_H_
