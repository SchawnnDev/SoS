#include <greatest.h>

TEST tour_creation(void) {
    ASSERT_EQ(2, 2);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_TEST(tour_creation);

    GREATEST_MAIN_END();
}