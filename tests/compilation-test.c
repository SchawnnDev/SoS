#include <greatest.h>
#include "log.h"
#include "compilation.h"

TEST checkWordIsIdTest(void) {
    ASSERT_EQ_FMT(1, checkWordIsId("var1"),"%d");
    ASSERT_EQ_FMT(0, checkWordIsId("&&&p--es"),"%d");
    ASSERT_EQ_FMT(0, checkWordIsId("+qsd"), "%d");
    PASS();
}


TEST checkWordIsIntTest(void) {
    ASSERT_EQ_FMT(1, checkWordIsInt("10"),"%d");
    ASSERT_EQ_FMT(1, checkWordIsInt("-10"),"%d");
    ASSERT_EQ_FMT(1, checkWordIsInt("+10"), "%d");
    ASSERT_EQ_FMT(0, checkWordIsInt("-a10"), "%d");
    ASSERT_EQ_FMT(0, checkWordIsInt("abz"), "%d");
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    log_set_quiet(true); // No logs in tests

    RUN_TEST(checkWordIsIdTest);
    RUN_TEST(checkWordIsIntTest);

    GREATEST_MAIN_END();
}

