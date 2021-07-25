#include <unity.h>

#include <cherry.h>
#include <grid.h>
#include <utils.h>

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
    uint8_t width  = 20;
    uint8_t height = 20;

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 5, .y = 0}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 5, .y = 0}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 7, .y = 0}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 7, .y = 0}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 4, .y = 2}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 4, .y = 2}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 15, .y = 2}));
    TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 15, .y = 2}));
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

void test__apply_splice_random() {
    uint8_t width  = 20;
    uint8_t height = 20;

    coord_t source = {.x = 0, .y = 0};
    coord_t target = {.x = (uint8_t)(width / 2), .y = (uint8_t)(height / 2)};

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    for (uint16_t i = 0; i < 1000; i++) {
        uint8_t x = get_random_number(width - 1);
        uint8_t y = get_random_number(height - 1);

        uint16_t original_distance = path_distance(graph_context, source, target);

        // Not all random splices are possible
        if (!_apply_splice(graph_context, (coord_t){.x = x, .y = y}))
            continue;

        TEST_ASSERT_FALSE(is_graph_fully_connected(graph_context));
        TEST_ASSERT_EQUAL(2, tag_paths(graph_context));

        TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = x, .y = y}));

        TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));
        TEST_ASSERT_EQUAL(1, tag_paths(graph_context));

        uint16_t final_distance = path_distance(graph_context, source, target);

        TEST_ASSERT_EQUAL(original_distance, final_distance);
    }

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

// Failing because it is getting stuck in a loop
void test__apply_splice_random_super() {
    uint8_t width  = 20;
    uint8_t height = 20;

    coord_t source = {.x = 0, .y = 0};
    coord_t target = {.x = (uint8_t)(width / 2), .y = (uint8_t)(height / 2)};

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);
    TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));

    for (uint16_t i = 0; i < 1000; i++) {
        uint8_t x = get_random_number(width - 1);
        uint8_t y = get_random_number(height - 1);

        uint16_t original_distance   = path_distance(graph_context, source, target);
        uint16_t original_path_count = tag_paths(graph_context);

        // Not all random splices are possible
        if (!_apply_splice(graph_context, (coord_t){.x = x, .y = y}))
            continue;

        TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = x, .y = y}));

        TEST_ASSERT_EQUAL(original_path_count, tag_paths(graph_context));
        TEST_ASSERT_EQUAL(original_distance, path_distance(graph_context, source, target));

        // Doing this results in several splices being applied on top of each other
        TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = x, .y = y}));
    }

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

// Test if path_distance doesn't get stuck in an infinite loop if target is
// unreachable from source
void test__apply_splice__path_distance_bug() {
    uint8_t width  = 4;
    uint8_t height = 4;

    coord_t source = {.x = 0, .y = 0};
    coord_t target = {.x = 2, .y = 2};

    grid_t *         grid          = create_grid(width, height);
    graph_context_t *graph_context = create_graph_context(grid);

    build_halmiton_with_dfs(graph_context);

    TEST_ASSERT_TRUE(_apply_splice(graph_context, (coord_t){.x = 1, .y = 2}));
    TEST_ASSERT_EQUAL(4294967295, path_distance(graph_context, source, target));

    destroy_graph_context(graph_context);
    destroy_grid(grid);
}

void test_perturbate_hamiltonian_cycle() {
    uint8_t width  = 20;
    uint8_t height = 20;

    /*coord_t source = {.x = 0, .y = 0};*/
    /*coord_t target = {.x = (uint8_t)(width / 2), .y = (uint8_t)(height / 2)};*/

    for (int i = 0; i < 20; i++) {
        grid_t *         grid          = create_grid(width, height);
        graph_context_t *graph_context = create_graph_context(grid);

        build_halmiton_with_dfs(graph_context);

        /*uint16_t original_distance = path_distance(graph_context, source, target);*/

        for (int j = 0; j < 250; j++) {
            TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));
            TEST_ASSERT_EQUAL(1, tag_paths(graph_context));

            /*bool result =*/
            perturbate_hamiltonian_cycle(graph_context);
            /*printf("%3d %3d %d\n", original_distance, path_distance(graph_context, source, target), result);*/
            /*printf("%3d %d\n", path_distance(graph_context, source, target), result);*/

            TEST_ASSERT_TRUE(is_graph_fully_connected(graph_context));
            TEST_ASSERT_EQUAL(1, tag_paths(graph_context));
        }

        destroy_graph_context(graph_context);
        destroy_grid(grid);
    }
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_build_halmiton_with_dfs_visits_all_cells);
    RUN_TEST(test__apply_splice);
    RUN_TEST(test__apply_splice__path_distance_bug);
    RUN_TEST(test__apply_splice_random);
    RUN_TEST(test__apply_splice_random_super);
    RUN_TEST(test_perturbate_hamiltonian_cycle);

    return UNITY_END();
}
