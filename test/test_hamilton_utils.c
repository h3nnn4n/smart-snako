#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/hamilton_utils.h>

void test_build_halmiton_with_dfs_visits_all_cells() {
    for (int width = 5; width <= 10; width++) {
        for (int height = 5; height <= 10; height++) {
            uint32_t not_visited_expected_count = 0;

            if (width % 2 == 1 && height % 2 == 1)
                not_visited_expected_count = 1;

            /*char message[512];*/
            /*snprintf(message, sizeof(message), "testing %d %d", width, height);*/
            /*TEST_MESSAGE(message);*/

            grid_t *         grid          = create_grid(width, height);
            graph_context_t *graph_context = create_graph_context(grid);

            build_halmiton_with_dfs(graph_context);

            TEST_ASSERT_EQUAL(not_visited_expected_count, cells_not_visited_count(graph_context));

            destroy_graph_context(graph_context);
            destroy_grid(grid);
        }
    }
}

void test__apply_splice() {
    uint8_t width  = 10;
    uint8_t height = 10;

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 3, .y = 0}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 3, .y = 0}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 5, .y = 0}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 5, .y = 0}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 4, .y = 2}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 4, .y = 2}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

void test_perturbate_hamiltonian_cycle() {
    //
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_build_halmiton_with_dfs_visits_all_cells);
    RUN_TEST(test_perturbate_hamiltonian_cycle);
    RUN_TEST(test__apply_splice);

    return UNITY_END();
}

