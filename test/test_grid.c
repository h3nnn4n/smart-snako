#include <unity.h>

#include <config.h>
#include <grid.h>

void test_create_grid() {
    grid_t *grid = NULL;

    grid = create_grid(10, 11);
    TEST_ASSERT_EQUAL_INT(grid->width, 10);
    TEST_ASSERT_EQUAL_INT(grid->height, 11);
    destroy_grid(grid);

    grid = create_grid(100, 101);
    TEST_ASSERT_EQUAL_INT(grid->width, 100);
    TEST_ASSERT_EQUAL_INT(grid->height, 101);
    destroy_grid(grid);
}

void test_game_over() {
    grid_t *grid = create_grid(10, 11);

    TEST_ASSERT_FALSE(is_game_over(grid));

    set_game_over(grid);

    TEST_ASSERT_TRUE(is_game_over(grid));

    destroy_grid(grid);
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_create_grid);
    RUN_TEST(test_game_over);

    return UNITY_END();
}
