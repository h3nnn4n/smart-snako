#include <stddef.h>

#include <entropy.h>
#include <pcg_variants.h>
#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/graph_utils.h>
#include <agents/dfs.h>

void _remove_snake_from_grid(grid_t *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->cells[x][y].has_snake) {
                grid->cells[x][y].has_snake     = false;
                grid->cells[x][y].snake_counter = 0;
            }
        }
    }
}

void test_graph_context() {
    for (int width = 5; width <= 10; width++) {
        for (int height = 5; height <= 10; height++) {
            grid_t *         grid  = create_grid(width, height);
            graph_context_t *graph = create_graph_context(grid);

            TEST_ASSERT_NOT_NULL(graph);
            TEST_ASSERT_NOT_NULL(graph->path);
            TEST_ASSERT_NOT_NULL(graph->grid);

            for (uint8_t i = 0; i < grid->width; i++) {
                TEST_ASSERT_NOT_NULL(graph->path[i]);
            }

            destroy_graph_context(graph);
            destroy_grid(grid);
        }
    }
}

void test_reset_graph_context() {
    for (int width = 5; width <= 10; width++) {
        for (int height = 5; height <= 10; height++) {
            grid_t *         grid  = create_grid(width, height);
            graph_context_t *graph = create_graph_context(grid);

            TEST_ASSERT_FALSE(all_cells_visited(graph));
            TEST_ASSERT_EQUAL(width * height, cells_not_visited_count(graph));

            for (int y = 0; y < grid->height; y++) {
                for (int x = 0; x < grid->width; x++) {
                    graph->path[x][y].visited = true;
                }
            }

            TEST_ASSERT_TRUE(all_cells_visited(graph));
            TEST_ASSERT_EQUAL(0, cells_not_visited_count(graph));

            reset_graph_context(graph);

            TEST_ASSERT_FALSE(all_cells_visited(graph));
            TEST_ASSERT_EQUAL(width * height, cells_not_visited_count(graph));

            for (int y = 0; y < grid->height; y++) {
                for (int x = 0; x < grid->width; x++) {
                    graph->path[x][y].visited = true;
                }
            }

            TEST_ASSERT_TRUE(all_cells_visited(graph));
            TEST_ASSERT_EQUAL(0, cells_not_visited_count(graph));

            destroy_graph_context(graph);
            destroy_grid(grid);
        }
    }
}

void test_cells_not_visited_count() {
    for (int width = 5; width <= 10; width++) {
        for (int height = 5; height <= 10; height++) {
            grid_t *         grid  = create_grid(width, height);
            graph_context_t *graph = create_graph_context(grid);

            TEST_ASSERT_EQUAL(width * height, cells_not_visited_count(graph));

            for (int y = 0; y < grid->height; y++) {
                for (int x = 0; x < grid->width; x++) {
                    graph->path[x][y].visited = true;
                }
            }

            TEST_ASSERT_EQUAL(0, cells_not_visited_count(graph));

            destroy_graph_context(graph);
            destroy_grid(grid);
        }
    }
}

void test_all_cells_visited() {
    for (int width = 5; width <= 10; width++) {
        for (int height = 5; height <= 10; height++) {
            grid_t *         grid  = create_grid(width, height);
            graph_context_t *graph = create_graph_context(grid);

            TEST_ASSERT_FALSE(all_cells_visited(graph));

            for (int y = 0; y < grid->height; y++) {
                for (int x = 0; x < grid->width; x++) {
                    graph->path[x][y].visited = true;
                }
            }

            TEST_ASSERT_TRUE(all_cells_visited(graph));

            destroy_graph_context(graph);
            destroy_grid(grid);
        }
    }
}

void test_set_target() {
    grid_t *         grid  = create_grid(15, 15);
    graph_context_t *graph = create_graph_context(grid);

    TEST_ASSERT_FALSE(graph->path[1][1].target);

    set_graph_target(graph, 1, 1);

    TEST_ASSERT_TRUE(graph->path[1][1].target);

    destroy_graph_context(graph);
    destroy_grid(grid);
}

void test_dfs() {
    for (int width = 5; width <= 20; width++) {
        for (int height = 5; height <= 20; height++) {

            uint8_t start_x = pcg32_boundedrand(width);
            uint8_t start_y = pcg32_boundedrand(height);
            uint8_t goal_x;
            uint8_t goal_y;

            grid_t *         grid  = create_grid(width, height);
            graph_context_t *graph = create_graph_context(grid);
            _remove_snake_from_grid(grid);

            grid->cells[start_x][start_y].has_snake     = true;
            grid->cells[start_x][start_y].snake_counter = 1;
            grid->snake_head_x                          = start_x;
            grid->snake_head_y                          = start_y;

            spawn_cherry(grid);
            get_cherry_position(grid, &goal_x, &goal_y);

            /*printf("\n");*/
            /*printf("%d %d\n", width, height);*/
            /*printf("%d %d %d %d\n", start_x, start_y, goal_x, goal_y);*/
            bool result = dfs(graph, start_x, start_y);

            TEST_ASSERT_TRUE(result);

            do {
                direction_t direction = graph->path[start_x][start_y].next_direction;

                switch (direction) {
                    case RIGHT: start_x++; break;
                    case LEFT: start_x--; break;
                    case UP: start_y--; break;
                    case DOWN: start_y++; break;
                }
            } while (graph->path[start_x][start_y].target != true);

            destroy_graph_context(graph);
            destroy_grid(grid);
        }
    }
}

void test_ida_dfs() {
    for (int width = 5; width <= 20; width++) {
        for (int height = 5; height <= 20; height++) {

            uint8_t start_x = pcg32_boundedrand(width);
            uint8_t start_y = pcg32_boundedrand(height);
            uint8_t goal_x;
            uint8_t goal_y;

            grid_t *         grid  = create_grid(width, height);
            graph_context_t *graph = create_graph_context(grid);
            _remove_snake_from_grid(grid);

            grid->cells[start_x][start_y].has_snake     = true;
            grid->cells[start_x][start_y].snake_counter = 1;
            grid->snake_head_x                          = start_x;
            grid->snake_head_y                          = start_y;

            spawn_cherry(grid);
            get_cherry_position(grid, &goal_x, &goal_y);

            /*printf("\n");*/
            /*printf("%d %d\n", width, height);*/
            /*printf("%d %d %d %d\n", start_x, start_y, goal_x, goal_y);*/
            bool result = ida_dfs(graph, start_x, start_y);

            TEST_ASSERT_TRUE(result);

            do {
                direction_t direction = graph->path[start_x][start_y].next_direction;

                switch (direction) {
                    case RIGHT: start_x++; break;
                    case LEFT: start_x--; break;
                    case UP: start_y--; break;
                    case DOWN: start_y++; break;
                }
            } while (graph->path[start_x][start_y].target != true);

            destroy_graph_context(graph);
            destroy_grid(grid);
        }
    }
}

void test_shuffle_directions() {
    for (int n_run = 0; n_run < 100; n_run++) {
        uint8_t     count[4]      = {0};
        direction_t directions[4] = {RIGHT, LEFT, UP, DOWN};

        for (int i = 0; i < 4; i++)
            count[directions[i]]++;

        for (int i = 0; i < 4; i++)
            TEST_ASSERT_EQUAL(1, count[i]);

        for (int shuffle_i = 0; shuffle_i <= n_run; shuffle_i++)
            shuffle_directions(directions, 4);

        for (int i = 0; i < 4; i++)
            count[directions[i]]--;

        for (int i = 0; i < 4; i++)
            TEST_ASSERT_EQUAL(0, count[i]);
    }
}

void setUp() {}
void tearDown() {}

int main() {
    /*uint64_t seeds[2];*/
    /*entropy_getbytes((void *)seeds, sizeof(seeds));*/
    /*pcg32_srandom(seeds[0], seeds[1]);*/
    pcg32_srandom(10u, 20u);

    UNITY_BEGIN();

    RUN_TEST(test_graph_context);
    RUN_TEST(test_cells_not_visited_count);
    RUN_TEST(test_all_cells_visited);
    RUN_TEST(test_reset_graph_context);

    RUN_TEST(test_set_target);
    RUN_TEST(test_shuffle_directions);

    RUN_TEST(test_dfs);
    RUN_TEST(test_ida_dfs);

    return UNITY_END();
}
