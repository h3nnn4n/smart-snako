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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "file_utils.h"
#include "stats.h"

stats_t *create_stats() {
    stats_t *stats = malloc(sizeof(stats_t));
    memset(stats, 0, sizeof(stats_t));

    return stats;
}

void destroy_stats(stats_t *stats) {
    if (stats->agent_name != NULL)
        free(stats->agent_name);

    free(stats);
}

void register_move(stats_t *stats) {
    stats->total_moves++;
    stats->moves_since_last_cherry++;
}

void register_cherry_eaten(stats_t *stats) {
    stats->cherries_eaten++;
    stats->moves_since_last_cherry = 0;
}

void register_agent_runtime(stats_t *stats, double runtime) { stats->agent_runtime = runtime; }

void print_stats(stats_t *stats) {
    if (!get_config()->verbose)
        return;

    printf("cherries_eaten: %4u    ", stats->cherries_eaten);
    printf("moves: %7u    ", stats->total_moves);
    printf("moves_since_last_cherry: %7u", stats->moves_since_last_cherry);

    printf("\n");
}

void dump_stats(stats_t *stats) {
    ensure_directory_exists("stats");

    grid_t *grid     = stats->grid;
    char *  filename = "stats/log.csv";
    FILE *  f        = NULL;

    if (!file_exists(filename)) {
        f = fopen(filename, "wt");
        fprintf(f, "agent_name,cherries_eaten,total_moves,agent_runtime,");
        fprintf(f, "max_moves_without_cherry,grid_width,grid_height");
        fprintf(f, "\n");
    } else {
        f = fopen(filename, "at");
    }

    fprintf(f, "%s,%u,%u,%f,", stats->agent_name, stats->cherries_eaten, stats->total_moves, stats->agent_runtime);
    fprintf(f, "%u,%u,%u", grid->max_moves_without_cherry, grid->width, grid->height);
    fprintf(f, "\n");

    fclose(f);
}

void set_agent_name(stats_t *stats, char *agent_name) {
    assert(stats->agent_name == NULL);
    assert(agent_name != NULL);

    stats->agent_name = malloc(sizeof(char) * (strlen(agent_name) + 5));

    snprintf(stats->agent_name, strlen(agent_name) + 1, "%s", agent_name);
}
