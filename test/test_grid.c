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

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_create_grid);

    return UNITY_END();
}
