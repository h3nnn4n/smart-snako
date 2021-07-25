#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/graph_utils.h>
#include <agents/hamilton_utils.h>

void test_path_distance() {
    uint8_t width  = 20;
    uint8_t height = 20;

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);

    TEST_ASSERT_EQUAL(10, path_distance(graph_context, (coord_t){.x = 0, .y = 0}, (coord_t){.x = 10, .y = 0}));
    TEST_ASSERT_EQUAL(5, path_distance(graph_context, (coord_t){.x = 2, .y = 0}, (coord_t){.x = 7, .y = 0}));
    TEST_ASSERT_EQUAL(114, path_distance(graph_context, (coord_t){.x = 2, .y = 2}, (coord_t){.x = 17, .y = 17}));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

// Test if path_distance doesn't get stuck in an infinite loop if target is
// unreachable from source
void test_path_distance__cycle_detection() {
    uint8_t width  = 4;
    uint8_t height = 4;

    coord_t source = {.x = 0, .y = 0};
    coord_t target = {.x = 2, .y = 2};

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 1, .y = 2}));
    TEST_ASSERT_EQUAL(65535, path_distance(graph_context, source, target));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

void test_is_graph_fully_connected() {
    for (uint8_t width = 5; width < 15; width++) {
        for (uint8_t height = 5; height < 15; height++) {
            grid_t *         grid          = create_grid(width, height);
            graph_context_t *graph_context = create_graph_context(grid);

            build_halmiton_with_dfs(graph_context);

            TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

            // This tests a case where a portion of the graph is excluded from
            // the path (note that it is a DAG)
            graph_context->path[2][0].next_direction = DOWN;
            TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

            // This tests where there is a loop somewhere that doesn't lead
            // back from where it started (note that it is a DAG)
            graph_context->path[2][0].next_direction = DOWN;
            graph_context->path[2][1].next_direction = UP;
            TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

            destroy_graph_context(graph_context);
            destroy_grid(grid);
        }
    }
}

void test_tag_paths_single() {
    // TODO: Test multiple grid sizes
    // TODO: Test with wonky grids (broken paths)

    uint8_t width  = 10;
    uint8_t height = 10;

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);

    TEST_ASSERT_EQUAL(1, tag_paths(graph_context));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

void test_tag_paths_spliced() {
    // TODO: Test multiple grid sizes
    // TODO: Test with wonky grids (broken paths)

    uint8_t width  = 10;
    uint8_t height = 10;

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 5, .y = 5}));

    TEST_ASSERT_EQUAL(2, tag_paths(graph_context));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_path_distance);
    RUN_TEST(test_path_distance__cycle_detection);
    RUN_TEST(test_is_graph_fully_connected);
    RUN_TEST(test_tag_paths_single);
    RUN_TEST(test_tag_paths_spliced);

    return UNITY_END();
}
