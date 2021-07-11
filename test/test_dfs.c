#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/agent_utils.h>

void test_graph_context() {
    grid_t *         grid  = create_grid(15, 15);
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

void test_cells_not_visited_count() {
    grid_t *         grid  = create_grid(15, 15);
    graph_context_t *graph = create_graph_context(grid);

    TEST_ASSERT_EQUAL(15 * 15, cells_not_visited_count(graph));

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            graph->path[x][y].visited = true;
        }
    }

    TEST_ASSERT_EQUAL(0, cells_not_visited_count(graph));

    destroy_graph_context(graph);
    destroy_grid(grid);
}

void test_all_cells_visited() {
    grid_t *         grid  = create_grid(15, 15);
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

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_graph_context);
    RUN_TEST(test_cells_not_visited_count);
    RUN_TEST(test_all_cells_visited);

    return UNITY_END();
}
