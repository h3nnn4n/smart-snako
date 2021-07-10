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

#include "config.h"
#include "grid.h"
#include "utils.h"
#include "agents/random_agent.h"

static config_t *config;

int main(int argc, char **argv) {
    init_config();
    config = get_config();

    uint8_t width  = 10;
    uint8_t height = 10;
    char *agent_name = NULL;

    direction_t (*agent)(grid_t*);

    struct option long_options[] = {{"verbose", no_argument, &config->verbose, 1},
                                    {"quiet", no_argument, &config->verbose, 0},
                                    {"agent", required_argument, NULL, 'a'},
                                    {"width", required_argument, NULL, 'w'},
                                    {"height", required_argument, NULL, 'h'},
                                    {0, 0, 0, 0}};

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

                agent_name = malloc(strlen(optarg) * sizeof(char));
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

    if (agent_name == NULL || strcmp(agent_name, "random") ==0) {
        agent = random_agent;
    } else {
        fprintf(stderr, "\"%s\" is not a valid agent\n", agent_name);
        return EXIT_FAILURE;
    }

    grid_t *grid = create_grid(width, height);

    print_grid(grid);
    while (!is_game_over(grid)) {
        direction_t direction = agent(grid);
        simulate(grid, direction);
        print_grid(grid);
    }

    destroy_grid(grid);

    return EXIT_SUCCESS;
}
