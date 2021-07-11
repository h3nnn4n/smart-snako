#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/agent_utils.h>

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
    grid_t *         grid  = create_grid(15, 15);
    graph_context_t *graph = create_graph_context(grid);

    set_graph_target(graph, 1, 1);
    bool result = dfs(graph, 10, 10, 25);

    TEST_ASSERT_TRUE(result);

    uint8_t x = 10;
    uint8_t y = 10;

    do {
        direction_t direction = graph->path[x][y].next_direction;

        switch (direction) {
            case RIGHT: x++; break;
            case LEFT: x--; break;
            case UP: y--; break;
            case DOWN: y++; break;
        }
    } while (graph->path[x][y].target != true);

    destroy_graph_context(graph);
    destroy_grid(grid);
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
    UNITY_BEGIN();

    RUN_TEST(test_graph_context);
    RUN_TEST(test_cells_not_visited_count);
    RUN_TEST(test_all_cells_visited);
    RUN_TEST(test_reset_graph_context);

    RUN_TEST(test_set_target);
    RUN_TEST(test_shuffle_directions);

    RUN_TEST(test_dfs);

    return UNITY_END();
}
