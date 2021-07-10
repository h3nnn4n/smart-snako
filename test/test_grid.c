#include <unity.h>

#include <config.h>
#include <grid.h>

void test_allocate_grid() {
    allocate_grid(10, 10);
    allocate_grid(100, 100);

    // Just ensures that nothing catches fire too easily
    TEST_ASSERT_TRUE(1);
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_allocate_grid);

    return UNITY_END();
}
