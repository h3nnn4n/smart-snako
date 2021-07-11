#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/raw_hamilton_agent.h>

void test_build_halmiton_with_dfs() {
    for (int width = 5; width <= 10; width++) {
        for (int height = 5; height <= 10; height++) {
            uint32_t not_visited_expected_count = 0;

            if (width % 2 == 1 && height % 2 == 1)
                not_visited_expected_count = 1;

            /*char message[512];*/
            /*snprintf(message, sizeof(message), "testing %d %d", width, height);*/
            /*TEST_MESSAGE(message);*/

            grid_t *grid = create_grid(width, height);
            raw_hamilton_agent_create(grid);

            TEST_ASSERT_EQUAL(not_visited_expected_count, cells_not_visited_count(grid));

            raw_hamilton_agent_destroy(grid);
            destroy_grid(grid);
        }
    }
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_build_halmiton_with_dfs);

    return UNITY_END();
}
