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

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_cherry);

    return UNITY_END();
}
