#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/raw_hamilton_agent.h>

void test_build_halmiton_with_dfs() {
    char message[512];

    for (int width = 6; width < 10; width++) {
        for (int height = 6; height < 10; height++) {
            snprintf(message, sizeof(message), "testing %d %d", width, height);
            TEST_MESSAGE(message);

            grid_t *grid = create_grid(width, height);
            raw_hamilton_agent_create(grid);

            TEST_ASSERT_TRUE(all_cells_visited(grid));

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
