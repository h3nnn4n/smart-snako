#include <unity.h>

#include <utils.h>

void test_get_random_direction() {
    for (int i = 0; i < 1000; i++) {
        uint8_t value = (uint8_t)get_random_direction();
        TEST_ASSERT_TRUE(value < 4);
    }
}

void test_get_random_number() {
    for (int max_value = 5; max_value < 200; max_value++) {
        for (int i = 0; i < 1000; i++) {
            uint8_t value = (uint8_t)get_random_number(max_value);
            TEST_ASSERT_TRUE(value < max_value);
        }
    }
}

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_get_random_direction);
    RUN_TEST(test_get_random_number);

    return UNITY_END();
}
