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

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_path_distance);

    return UNITY_END();
}
