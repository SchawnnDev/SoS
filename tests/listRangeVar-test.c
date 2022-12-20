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

//----------------------//
// initVariablePosition //
//----------------------//
TEST initVariablePositionWithoutIdentifierFoundTest(void) {
    VariablePosition addr = initVariablePosition(NULL,NOTFOUND);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(NOTFOUND, addr->indexIdentifier,"%d");
    ASSERT_EQ(NULL, addr->rangePosition);
    PASS();
}

TEST initVariablePositionWithIdentifierFoundTest(void) {
    RangeVariable rangeVariable = initRangeVariable(0,NULL);
    VariablePosition addr = initVariablePosition(rangeVariable,10);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(10, addr->indexIdentifier,"%d");
    ASSERT_EQ(rangeVariable, addr->rangePosition);
    PASS();
}
//-----------------------//
// cleanVariablePosition //
//-----------------------//
TEST cleanVariablePositionTest(void) {
    cleanVariablePosition(initVariablePosition(NULL,UNSET));
    PASS();
}


//-----------------------//
// initListRangeVariable //
//-----------------------//
TEST initListRangeVariableTest(void) {
    ListRangeVariable addr = initListRangeVariable();

    ASSERT_NEQ(NULL, addr);
    ASSERT_NEQ(NULL, addr->cursor);
    PASS();
}

//------------------------//
// cleanListRangeVariable //
//------------------------//
TEST cleanListRangeVariableTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    cleanListRangeVariable(addr);
    PASS();
}

//------------------//
// addRangeVariable //
//------------------//
TEST addRangeVariableTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    RangeVariable lastCursor = addr->cursor;
    int lastLvl = addr->cursor->rangeLevel;

    ASSERT_EQ_FMT(RETURN_SUCCESS, addRangeVariable(addr),"%d");
    ASSERT_NEQ(NULL, addr->cursor);
    ASSERT_NEQ(lastCursor, addr->cursor);
    ASSERT_EQ( addr->cursor,lastCursor->nextLevel);
    ASSERT_EQ( lastCursor,addr->cursor->previousLevel);
    ASSERT_EQ_FMT(lastLvl+1, addr->cursor->rangeLevel,"%d");
    PASS();
}

//---------------------//
// deleteRangeVariable //
//---------------------//
TEST deleteRangeVariableWithoutPreviousLvlTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    RangeVariable lastCursor = addr->cursor;
    int lastLvl = addr->cursor->rangeLevel;

    ASSERT_EQ_FMT(RETURN_FAILURE, deleteRangeVariable(addr),"%d");
    ASSERT_NEQ(NULL, addr->cursor);
    ASSERT_EQ(lastCursor, addr->cursor);
    ASSERT_EQ_FMT(lastLvl, addr->cursor->rangeLevel,"%d");
    PASS();
}

TEST deleteRangeVariableWithNextLvlTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    RangeVariable lastCursor = addr->cursor;
    int lastLvl = addr->cursor->rangeLevel;

    addRangeVariable(addr);

    ASSERT_EQ_FMT(RETURN_SUCCESS, deleteRangeVariable(addr),"%d");
    ASSERT_NEQ(NULL, addr->cursor);
    ASSERT_EQ(lastCursor, addr->cursor);
    ASSERT_EQ( NULL,lastCursor->nextLevel);
    ASSERT_EQ_FMT(lastLvl, addr->cursor->rangeLevel,"%d");
    PASS();
}

TEST deleteRangeVariableWithNextLvlWithValuesTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    RangeVariable lastCursor = addr->cursor;
    int lastLvl = addr->cursor->rangeLevel;

    addRangeVariable(addr);
    addIdentifier(addr,"test");

    ASSERT_EQ_FMT(RETURN_SUCCESS, deleteRangeVariable(addr),"%d");
    ASSERT_NEQ(NULL, addr->cursor);
    ASSERT_EQ(lastCursor, addr->cursor);
    ASSERT_EQ( NULL,lastCursor->nextLevel);
    ASSERT_EQ_FMT(lastLvl, addr->cursor->rangeLevel,"%d");
    PASS();
}


GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_TEST(initRangeVariableLvlZeroTest);
    RUN_TEST(initRangeVariableLvlNTest);

    RUN_TEST(initVariablePositionWithoutIdentifierFoundTest);
    RUN_TEST(initVariablePositionWithIdentifierFoundTest);
    RUN_TEST(cleanVariablePositionTest);

    RUN_TEST(initListRangeVariableTest);
    RUN_TEST(cleanListRangeVariableTest);

    RUN_TEST(addRangeVariableTest);

    RUN_TEST(deleteRangeVariableWithoutPreviousLvlTest);
    RUN_TEST(deleteRangeVariableWithNextLvlTest);
    RUN_TEST(deleteRangeVariableWithNextLvlWithValuesTest);

    GREATEST_MAIN_END();
}

