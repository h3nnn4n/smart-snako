#include <unity.h>

#include <cherry.h>
#include <grid.h>

#include <agents/smart_snako_agent.h>

void test_agent() {
    char *  agent_name = "smart-snako";
    uint8_t width      = 20;
    uint8_t height     = 20;

    grid_t *grid = create_grid(width, height);
    set_agent_name(grid->stats, agent_name);
    spawn_cherry(grid);
    smart_snako_agent_create(grid);

    while (!is_game_over(grid)) {
        print_grid(grid);
        print_stats(grid->stats);
        direction_t direction = smart_snako_agent(grid);
        simulate(grid, direction);
    }

    TEST_ASSERT_TRUE(grid->stats->cherries_eaten > 10);

    smart_snako_agent_destroy(grid);
    destroy_grid(grid);
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_agent);

    return UNITY_END();
}
