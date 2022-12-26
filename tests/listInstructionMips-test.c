#include <greatest.h>
#include "listInstructionMips.h"

char* codes[IDEN_MAX] = {"test0","test1","test2","test3","test4","test5","test6","test7","test8","test9",
                         "test10","test11","test12","test13","test14","test15","test16","test17","test18","test19",
                         "test20","test21","test22","test23","test24","test25","test26","test27","test28","test29",
                         "test30","test31","test32","test33","test34","test35","test36","test37","test38","test39",
                         "test40","test41","test42","test43","test44","test45","test46","test47","test48","test49",
                         "test50","test51","test52","test53","test54","test55","test56","test57","test58","test59",
                         "test60","test61","test62","test63","test64","test65","test66","test67","test68","test69",
                         "test70","test71","test72","test73","test74","test75","test76","test77","test78","test79",
                         "test80","test81","test82","test83","test84","test85","test86","test87","test88","test89",
                         "test90","test91","test92","test93","test94","test95","test96","test97","test98","test99"};

//----------//
// initData //
//----------//
TEST initDataTest(void) {
    Data addr = initData(NULL);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->previousData);
    ASSERT_EQ(NULL, addr->nextData);
    ASSERT_NEQ(NULL, addr->lineData);
    ASSERT_EQ_FMT(0, addr->numberData,"%d");
    PASS();
}

//----------//
// initCode //
//----------//
TEST initCodeTest(void) {
    Code addr = initCode(NULL);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->previousCode);
    ASSERT_EQ(NULL, addr->nextCode);
    ASSERT_NEQ(NULL, addr->lineCode);
    ASSERT_NEQ(NULL, addr->unDefineGoto);
    ASSERT_EQ_FMT(0, addr->numberCode,"%d");
    ASSERT_EQ_FMT(0, addr->numberGoto,"%d");
    PASS();
}

//---------------------//
// initListInstruction //
//---------------------//
TEST initListInstructionTest(void) {
    ListInstruction addr = initListInstruction();

    ASSERT_NEQ(NULL, addr);
    ASSERT_NEQ(NULL, addr->cursorCode);
    ASSERT_NEQ(NULL, addr->cursorData);
    PASS();
}

//-----------//
// cleanData //
//-----------//
TEST cleanDataTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoData(addr,"test");
    cleanData(addr->cursorData);

    PASS();
}

//-----------//
// cleanCode //
//-----------//
TEST cleanCodeTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoCode(addr,"test");
    cleanCode(addr->cursorCode);

    PASS();
}

//----------------------//
// cleanListInstruction //
//----------------------//
TEST cleanListInstructionTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoCode(addr,"test");
    addIntoData(addr,"test");
    cleanListInstruction(addr);

    PASS();
}

//-------------//
// addIntoData //
//-------------//
TEST addIntoDataEmptyTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoData(addr,"test");

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->cursorData->previousData);
    ASSERT_EQ_FMT(1, addr->cursorData->numberData,"%d");
    ASSERT_STR_EQ("test", addr->cursorData->lineData[addr->cursorData->numberData-1]);
    PASS();
}

TEST addIntoDataFullTest(void) {
    ListInstruction addr = initListInstruction();

    //simule le remplissage de la liste
    int i;
    for(i = 0; i < DATA_TAB_MAX; i++){
        addIntoData(addr, codes[i]);
    }

    addIntoData(addr,"test");

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(1, addr->cursorData->numberData,"%d");
    ASSERT_STR_EQ("test", addr->cursorData->lineData[addr->cursorData->numberData-1]);
    ASSERT_NEQ(NULL, addr->cursorData->previousData);
    ASSERT_EQ_FMT(DATA_TAB_MAX, addr->cursorData->previousData->numberData,"%d");
    PASS();
}

//-------------//
// addIntoCode //
//-------------//
TEST addIntoCodeEmptyTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoCode(addr,"test");

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->cursorCode->previousCode);
    ASSERT_EQ_FMT(1, addr->cursorCode->numberCode,"%d");
    ASSERT_STR_EQ("test", addr->cursorCode->lineCode[addr->cursorCode->numberCode-1]);
    PASS();
}

TEST addIntoCodeFullTest(void) {
    ListInstruction addr = initListInstruction();

    //simule le remplissage de la liste
    int i;
    for(i = 0; i < CODE_TAB_MAX; i++){
        addIntoCode(addr, codes[i]);
    }

    addIntoCode(addr,"test");

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(1, addr->cursorCode->numberCode,"%d");
    ASSERT_STR_EQ("test", addr->cursorCode->lineCode[addr->cursorCode->numberCode-1]);
    ASSERT_NEQ(NULL, addr->cursorCode->previousCode);
    ASSERT_EQ(NULL, addr->cursorCode->nextCode);
    ASSERT_NEQ(NULL, addr->cursorCode->previousCode->nextCode);
    ASSERT_EQ_FMT(CODE_TAB_MAX, addr->cursorCode->previousCode->numberCode,"%d");
    PASS();
}

//----------------------//
// addIntoCodeWithIndex //
//----------------------//
TEST addIntoCodeWithIndexBadIndexTest(void) {
    ListInstruction addr = initListInstruction();

    ASSERT_EQ_FMT(RETURN_FAILURE, addIntoCodeWithIndex(addr->cursorCode,"test",-1),"%d");
    ASSERT_EQ_FMT(RETURN_FAILURE, addIntoCodeWithIndex(addr->cursorCode,"test",CODE_TAB_MAX),"%d");
    PASS();
}

//---------------------//
// addIntoUnDefineGoto //
//---------------------//
TEST addIntoUnDefineGotoTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoUnDefineGoto(addr);

    ASSERT_EQ_FMT(1, addr->cursorCode->numberCode,"%d");
    ASSERT_EQ_FMT(1, addr->cursorCode->numberGoto,"%d");
    ASSERT_EQ_FMT(0, addr->cursorCode->unDefineGoto[0],"%d");
    PASS();
}

//----------------------//
// completeUnDefineGoto //
//----------------------//
TEST completeUnDefineGotoSameTableTest(void) {
    ListInstruction addr = initListInstruction();
    addIntoUnDefineGoto(addr);
    addIntoCode(addr,"test");
    addIntoUnDefineGoto(addr);
    completeUnDefineGoto(addr,"unDefineGoto");

    ASSERT_EQ_FMT(3, addr->cursorCode->numberCode,"%d");
    ASSERT_EQ_FMT(0, addr->cursorCode->numberGoto,"%d");
    ASSERT_STR_EQ("unDefineGoto", addr->cursorCode->lineCode[0]);
    ASSERT_STR_EQ("test", addr->cursorCode->lineCode[1]);
    ASSERT_STR_EQ("unDefineGoto", addr->cursorCode->lineCode[2]);
    PASS();
}

TEST completeUnDefineGotoDifferentTableTest(void) {
    ListInstruction addr = initListInstruction();

    //simule le remplissage de la liste
    int i;
    for(i = 0; i < CODE_TAB_MAX-1; i++){
        addIntoCode(addr, codes[i]);
    }

    addIntoUnDefineGoto(addr);
    addIntoCode(addr,"test");
    addIntoUnDefineGoto(addr);
    completeUnDefineGoto(addr,"unDefineGoto");

    ASSERT_EQ_FMT(2, addr->cursorCode->numberCode,"%d");
    ASSERT_EQ_FMT(0, addr->cursorCode->numberGoto,"%d");
    ASSERT_EQ_FMT(0, addr->cursorCode->previousCode->numberGoto,"%d");
    ASSERT_STR_EQ("unDefineGoto", addr->cursorCode->previousCode->lineCode[CODE_TAB_MAX-1]);
    ASSERT_STR_EQ("test", addr->cursorCode->lineCode[0]);
    ASSERT_STR_EQ("unDefineGoto", addr->cursorCode->lineCode[1]);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    log_set_quiet(true); // No logs in tests

    RUN_TEST(initDataTest);
    RUN_TEST(initCodeTest);
    RUN_TEST(initListInstructionTest);

    RUN_TEST(cleanDataTest);
    RUN_TEST(cleanCodeTest);
    RUN_TEST(cleanListInstructionTest);

    RUN_TEST(addIntoDataEmptyTest);
    RUN_TEST(addIntoDataFullTest);

    RUN_TEST(addIntoCodeEmptyTest);
    RUN_TEST(addIntoCodeFullTest);
    RUN_TEST(addIntoCodeWithIndexBadIndexTest);

    RUN_TEST(addIntoUnDefineGotoTest);

    RUN_TEST(completeUnDefineGotoSameTableTest);
    RUN_TEST(completeUnDefineGotoDifferentTableTest);

    GREATEST_MAIN_END();
}

