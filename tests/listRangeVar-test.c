#include <greatest.h>
#include "listRangeVar.h"

char* names[IDEN_MAX] = {"test0","test1","test2","test3","test4","test5","test6","test7","test8","test9",
                         "test10","test11","test12","test13","test14","test15","test16","test17","test18","test19",
                         "test20","test21","test22","test23","test24","test25","test26","test27","test28","test29",
                         "test30","test31","test32","test33","test34","test35","test36","test37","test38","test39",
                         "test40","test41","test42","test43","test44","test45","test46","test47","test48","test49",
                         "test50","test51","test52","test53","test54","test55","test56","test57","test58","test59",
                         "test60","test61","test62","test63","test64","test65","test66","test67","test68","test69",
                         "test70","test71","test72","test73","test74","test75","test76","test77","test78","test79",
                         "test80","test81","test82","test83","test84","test85","test86","test87","test88","test89",
                         "test90","test91","test92","test93","test94","test95","test96","test97","test98","test99"};
ListInstruction listInstruction;

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
    addIdentifier(addr, "test");

    ASSERT_EQ_FMT(RETURN_SUCCESS, deleteRangeVariable(addr),"%d");
    ASSERT_NEQ(NULL, addr->cursor);
    ASSERT_EQ(lastCursor, addr->cursor);
    ASSERT_EQ( NULL,lastCursor->nextLevel);
    ASSERT_EQ_FMT(lastLvl, addr->cursor->rangeLevel,"%d");
    PASS();
}

//--------------------------//
// searchIdentifierPosition //
//--------------------------//
TEST searchIdentifierPositionWithoutIdentifierInRangeListTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    VariablePosition variablePosition = searchIdentifierPosition(addr,"test");

    ASSERT_EQ(NOTFOUND, variablePosition->indexIdentifier);
    ASSERT_EQ(NULL, variablePosition->rangePosition);
    PASS();
}

TEST searchIdentifierPositionWithIdentifierInRangeListWithOnlyOneLvlTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    addIdentifier(addr, "test");
    VariablePosition variablePosition = searchIdentifierPosition(addr,"test");

    ASSERT_EQ(0, variablePosition->indexIdentifier);
    ASSERT_EQ(addr->cursor, variablePosition->rangePosition);
    PASS();
}

TEST searchIdentifierPositionWithIdentifierInRangeListWithMayLvlTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    addRangeVariable(addr);
    addRangeVariable(addr);
    addIdentifier(addr, "test");
    VariablePosition variablePosition = searchIdentifierPosition(addr,"test");

    ASSERT_EQ(0, variablePosition->indexIdentifier);
    ASSERT_EQ(addr->cursor, variablePosition->rangePosition);
    PASS();
}

TEST searchIdentifierPositionWithIdentifierOnFirstLvlInRangeListWithMayLvlTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    RangeVariable firstRange = addr->cursor;
    addIdentifier(addr, "test");
    addRangeVariable(addr);
    addRangeVariable(addr);
    VariablePosition variablePosition = searchIdentifierPosition(addr,"test");

    ASSERT_EQ(0, variablePosition->indexIdentifier);
    ASSERT_EQ(firstRange, variablePosition->rangePosition);
    PASS();
}

TEST searchIdentifierPositionWithIdentifierOnNLvlInRangeListWithMayLvlAndNotFirstIdentifierTest(void) {
    ListRangeVariable addr = initListRangeVariable();
    addRangeVariable(addr);
    RangeVariable firstRange = addr->cursor;
    addIdentifier(addr, "hello");
    addIdentifier(addr, "test");
    addRangeVariable(addr);
    VariablePosition variablePosition = searchIdentifierPosition(addr,"test");

    ASSERT_EQ(1, variablePosition->indexIdentifier);
    ASSERT_EQ(firstRange, variablePosition->rangePosition);
    PASS();
}

//--------------------------//
// addIdentifier //
//--------------------------//
TEST addIdentifierTest(void) {
    ListRangeVariable addr = initListRangeVariable();

    ASSERT_EQ(RETURN_SUCCESS, addIdentifier(addr, "test"));
    VariablePosition variablePosition = searchIdentifierPosition(addr,"test");
    ASSERT_NEQ(NULL, variablePosition->rangePosition);
    ASSERT_EQ(0, variablePosition->indexIdentifier);
    PASS();
}

TEST addIdentifierTwiceTest(void) {
    ListRangeVariable addr = initListRangeVariable();

    ASSERT_EQ(RETURN_SUCCESS, addIdentifier(addr, "test"));
    ASSERT_EQ(RETURN_FAILURE, addIdentifier(addr, "test"));
    PASS();
}
TEST addIdentifierToManyIdentifierTest(void) {
    ListRangeVariable addr = initListRangeVariable();

    //simule le remplissage de la liste
    int i;
    for(i = 0; i < IDEN_MAX; i++){
        addIdentifier(addr, names[i]);
    }

    ASSERT_EQ(RETURN_FAILURE, addIdentifier(addr, "test"));
    PASS();
}


GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    log_set_quiet(false); // No logs in tests
    listInstruction = initListInstruction();

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

    RUN_TEST(searchIdentifierPositionWithoutIdentifierInRangeListTest);
    RUN_TEST(searchIdentifierPositionWithIdentifierInRangeListWithOnlyOneLvlTest);
    RUN_TEST(searchIdentifierPositionWithIdentifierInRangeListWithMayLvlTest);
    RUN_TEST(searchIdentifierPositionWithIdentifierOnFirstLvlInRangeListWithMayLvlTest);
    RUN_TEST(searchIdentifierPositionWithIdentifierOnNLvlInRangeListWithMayLvlAndNotFirstIdentifierTest);

    RUN_TEST(addIdentifierTest);
    RUN_TEST(addIdentifierTwiceTest);
    RUN_TEST(addIdentifierToManyIdentifierTest);

    GREATEST_MAIN_END();
}

