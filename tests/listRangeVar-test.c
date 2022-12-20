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

TEST initRangeVariableLvlNTest(void) {
    RangeVariable addr1 = initRangeVariable(0,NULL);
    RangeVariable addr = initRangeVariable(10,addr1);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(10, addr->rangeLevel,"%d");
    ASSERT_EQ(NULL, addr->nextLevel);
    ASSERT_EQ(addr1, addr->previousLevel);
    ASSERT_NEQ(NULL, addr->listIdentifier);
    PASS();
}

//-------------------//
// initListRangeVariable //
//-------------------//
TEST initListRangeVariableTest(void) {
    ListRangeVariable addr = initListRangeVariable(0,NULL);

    ASSERT_NEQ(NULL, addr);
    ASSERT_NEQ(NULL, addr->cursor);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_TEST(initRangeVariableLvlZeroTest);
    RUN_TEST(initRangeVariableLvlNTest);

    RUN_TEST(initListRangeVariableTest);

    GREATEST_MAIN_END();
}

