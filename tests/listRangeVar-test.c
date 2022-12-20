#include <greatest.h>
#include "listRangeVar.h"

//-------------------//
// initRangeVariable //
//-------------------//
TEST initRangeVariableLvlZeroTest(void) {
    RangeVariable addr = initRangeVariable(0,NULL);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(0, addr->rangeLevel,"%d");
    ASSERT_EQ(NULL, addr->nextLevel);
    ASSERT_EQ(NULL, addr->previousLevel);
    ASSERT_NEQ(NULL, addr->listIdentifier);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_TEST(initRangeVariableLvlZeroTest);

    GREATEST_MAIN_END();
}

