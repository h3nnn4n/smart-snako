/*
 * Copyright <2021> <Renan S Silva, aka h3nnn4n>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "sum.h"

static config_t *config;

int main(int argc, char **argv) {
    init_config();
    config = get_config();

    struct option long_options[] = {{"verbose", no_argument, &config->verbose, 1},
                                    {"quiet", no_argument, &config->verbose, 0},
                                    {"fail-on-match", required_argument, 0, 'f'},
                                    {"say-hi", optional_argument, 0, 'h'},
                                    {0, 0, 0, 0}};

    int option_index = 0;

    while (1) {
        int c;

        c = getopt_long(argc, argv, "vqh::", long_options, &option_index);

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

            case 'f': {
                config->fail_on_match       = 1;
                config->fail_on_match_value = atoi(optarg);
            } break;

            case 'h': {
                config->say_hi = 1;

                if (optarg != NULL) {
                    config->name = malloc(strlen(optarg) * sizeof(char));
                    memcpy(config->name, optarg, sizeof(char) * (strlen(optarg)));
                }
            } break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                printf("%c %d %x", c, c, c);
                abort();
                break;
        }
    }

    if (config->say_hi) {
        if (config->name != NULL) {
            printf("hi %s\n", config->name);
        } else {
            printf("hi\n");
        }
    }

    // TODO(h3nnn4n): Ensure argv is long enough
    int x = atoi(argv[option_index++]);
    int y = atoi(argv[option_index++]);

    int result = sum(x, y);

    if (config->fail_on_match && config->fail_on_match_value == result) {
        if (config->verbose)
            fprintf(stderr, "result matched %d.\naborting!\n", result);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
