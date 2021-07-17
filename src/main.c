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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <entropy.h>
#include <pcg_variants.h>

#include "agents/dfs_agent.h"
#include "agents/ida_dfs_agent.h"
#include "agents/manhattan_agent.h"
#include "agents/random_agent.h"
#include "agents/raw_hamilton_agent.h"
#include "agents/smart_snako_agent.h"

#include "cherry.h"
#include "config.h"
#include "grid.h"
#include "utils.h"

static config_t *config;

int main(int argc, char **argv) {
    uint64_t seeds[2];
    entropy_getbytes((void *)seeds, sizeof(seeds));
    pcg32_srandom(seeds[0], seeds[1]);

    init_config();
    config = get_config();

    uint8_t width      = 10;
    uint8_t height     = 10;
    char *  agent_name = NULL;

    void (*agent_create)(grid_t *)  = NULL;
    void (*agent_destroy)(grid_t *) = NULL;
    direction_t (*agent)(grid_t *)  = NULL;

    struct option long_options[] = {
        {"verbose", no_argument, &config->verbose, 1}, {"quiet", no_argument, &config->verbose, 0},
        {"agent", required_argument, NULL, 'a'},       {"width", required_argument, NULL, 'w'},
        {"height", required_argument, NULL, 'h'},      {0, 0, 0, 0}};

    int option_index = 0;

    while (1) {
        int c;

        c = getopt_long(argc, argv, "vqa:h:w:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;

                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);

                printf("\n");
                break;

            case 'a': {
                assert(optarg != NULL);

                // Deallocate, so we can reallocate it again with the right size
                if (agent_name != NULL)
                    free(agent_name);

                agent_name = malloc(strlen(optarg) * sizeof(char) + 1);

                memcpy(agent_name, optarg, sizeof(char) * (strlen(optarg)));
            } break;

            case 'w':
                assert(optarg != NULL);
                width = atoi(optarg);
                break;

            case 'h':
                assert(optarg != NULL);
                height = atoi(optarg);
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                printf("%c %d %x", c, c, c);
                abort();
                break;
        }
    }

    if (agent_name == NULL) {
        char *name = "random";
        agent_name = malloc(7);
        memcpy(agent_name, name, 6);
        agent_name[6] = '\0';
    }

    if (strcmp(agent_name, "random") == 0) {
        agent         = random_agent;
        agent_create  = random_agent_create;
        agent_destroy = random_agent_destroy;
    } else if (strcmp(agent_name, "manhattan") == 0) {
        agent         = manhattan_agent;
        agent_create  = manhattan_agent_create;
        agent_destroy = manhattan_agent_destroy;
    } else if (strcmp(agent_name, "raw_hamilton") == 0) {
        agent         = raw_hamilton_agent;
        agent_create  = raw_hamilton_agent_create;
        agent_destroy = raw_hamilton_agent_destroy;
    } else if (strcmp(agent_name, "dfs") == 0) {
        agent         = dfs_agent;
        agent_create  = dfs_agent_create;
        agent_destroy = dfs_agent_destroy;
    } else if (strcmp(agent_name, "ida_dfs") == 0) {
        agent         = ida_dfs_agent;
        agent_create  = ida_dfs_agent_create;
        agent_destroy = ida_dfs_agent_destroy;
    } else if (strcmp(agent_name, "smart_snako") == 0) {
        agent         = smart_snako_agent;
        agent_create  = smart_snako_agent_create;
        agent_destroy = smart_snako_agent_destroy;
    } else {
        fprintf(stderr, "\"%s\" is not a valid agent\n", agent_name);
        free(agent_name);
        return EXIT_FAILURE;
    }

    grid_t *grid = create_grid(width, height);
    set_agent_name(grid->stats, agent_name);
    spawn_cherry(grid);
    agent_create(grid);

    while (!is_game_over(grid)) {
        print_grid(grid);
        print_stats(grid->stats);
        direction_t direction = agent(grid);
        simulate(grid, direction);
    }

    dump_stats(grid->stats);

    agent_destroy(grid);
    destroy_grid(grid);

    if (agent_name != NULL)
        free(agent_name);

    return EXIT_SUCCESS;
}
