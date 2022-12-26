//
// Created by Quentin on 17/12/2022.
//
#include <greatest.h>
#include "listTmp.h"

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

//-------------------//
// initTmpValuesTest //
//-------------------//
TEST initTmpValuesTest(void) {
    TmpValues addr = initTmpValues(NULL);

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->previousTmpValues);
    ASSERT_EQ_FMT(0, addr->numberValues,"%d");
    PASS();
}

//------------------------//
// initListIdentifierTest //
//------------------------//
TEST initListTmpTest(void) {
    ListTmp addr = initListTmp();

    ASSERT_NEQ(NULL, addr);
    ASSERT_NEQ(NULL, addr->cursor);
    PASS();
}

//------------------//
// cleanListTmpTest //
//------------------//
TEST cleanListTmpTest(void) {
    ListTmp addr = initListTmp();
    cleanListTmp(addr);

    PASS();
}

//------------------------//
// addIntoListIdentifier //
//------------------------//
TEST addIntoListTmpTest(void) {
    ListTmp addr = initListTmp();
    char* name = "test";
    //simule le remplissage de la liste
    int i;
    for(i = 0; i < TMP_TAB_MAX; i++){
        addIntoListTmp(addr, names[i]);
    }

    ASSERT_EQ_FMT(RETURN_FAILURE, addIntoListTmp(addr, name),"%d");
    ASSERT_EQ_FMT(TMP_TAB_MAX, addr->cursor->numberValues,"%d");
    PASS();
}

//------------//
// addListTmp //
//------------//
TEST addListTmpTest(void) {
    ListTmp addr = initListTmp();
    TmpValues lastCursor = addr->cursor;
    addListTmp(addr, initTmpValues(addr->cursor));

    ASSERT_EQ_FMT(RETURN_SUCCESS, addIntoListTmp(addr, "test"),"%d");
    ASSERT_EQ_FMT(1, addr->cursor->numberValues,"%d");
    ASSERT_NEQ(lastCursor, addr->cursor);
    ASSERT_EQ(lastCursor, addr->cursor->previousTmpValues);
    PASS();
}

//---------------//
// deleteListTmp //
//---------------//
TEST deleteListTmpEmptyTest(void) {
    ListTmp addr = initListTmp();
    deleteListTmp(addr);

    ASSERT_EQ_FMT(RETURN_FAILURE, deleteListTmp(addr),"%d");
    PASS();
}

TEST deleteListTmpTest(void) {
    ListTmp addr = initListTmp();

    ASSERT_EQ_FMT(RETURN_SUCCESS, deleteListTmp(addr),"%d");
    ASSERT_EQ(NULL, addr->cursor);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    log_set_quiet(true); // No logs in tests

    RUN_TEST(initTmpValuesTest);
    RUN_TEST(initListTmpTest);
    RUN_TEST(cleanListTmpTest);

    RUN_TEST(addIntoListTmpTest);
    RUN_TEST(addListTmpTest);

    RUN_TEST(deleteListTmpEmptyTest);
    RUN_TEST(deleteListTmpTest);

    GREATEST_MAIN_END();
}

