#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/dfs_agent.h>

void test_agent() {
    char *agent_name = "dfs";
    for (int width = 20; width <= 25; width++) {
        for (int height = 20; height <= 25; height++) {
            grid_t *grid = create_grid(width, height);
            set_agent_name(grid->stats, agent_name);
            spawn_cherry(grid);
            dfs_agent_create(grid);

            while (!is_game_over(grid)) {
                print_grid(grid);
                print_stats(grid->stats);
                direction_t direction = dfs_agent(grid);
                simulate(grid, direction);
            }

            TEST_ASSERT_TRUE(grid->stats->cherries_eaten > 5);

            dfs_agent_destroy(grid);
            destroy_grid(grid);
        }
    }
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_agent);

    return UNITY_END();
}
