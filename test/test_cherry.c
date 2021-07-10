#include <unity.h>

#include <cherry.h>
#include <grid.h>

void test_cherry() {
    grid_t *grid = create_grid(30, 30);
    TEST_ASSERT_FALSE(has_cherry(grid));

    spawn_cherry(grid);

    TEST_ASSERT_TRUE(has_cherry(grid));

    destroy_grid(grid);
}

void test_get_cherry_position_returns_false() {
    uint8_t x, y;
    grid_t *grid = create_grid(30, 30);
    TEST_ASSERT_FALSE(has_cherry(grid));
    TEST_ASSERT_FALSE(get_cherry_position(grid, &x, &y));

    destroy_grid(grid);
}

void test_get_cherry_position_returns_true() {
    uint8_t x, y;
    grid_t *grid = create_grid(30, 30);
    TEST_ASSERT_FALSE(has_cherry(grid));

    spawn_cherry(grid);

    TEST_ASSERT_TRUE(get_cherry_position(grid, &x, &y));

    destroy_grid(grid);
}

void test_get_cherry_position() {
    uint8_t x, y;
    grid_t *grid = create_grid(30, 30);
    TEST_ASSERT_FALSE(has_cherry(grid));

    grid->cells[4][2].has_cherry = true;

    TEST_ASSERT_TRUE(get_cherry_position(grid, &x, &y));

    TEST_ASSERT_EQUAL(x, 4);
    TEST_ASSERT_EQUAL(y, 2);

    destroy_grid(grid);
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_cherry);
    RUN_TEST(test_get_cherry_position_returns_true);
    RUN_TEST(test_get_cherry_position_returns_false);
    RUN_TEST(test_get_cherry_position);

    return UNITY_END();
}
