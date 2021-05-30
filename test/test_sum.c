#include <unity.h>

#include <config.h>
#include <sum.h>

void test_one_plus_two_equals_three() {
    int x      = 1;
    int y      = 2;
    int result = sum(x, y);

    TEST_ASSERT_TRUE(result == 3);
}

void setUp() { init_config(); }
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_one_plus_two_equals_three);

    return UNITY_END();
}
