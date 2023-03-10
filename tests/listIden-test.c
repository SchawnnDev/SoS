//
// Created by Quentin on 17/12/2022.
//
#include <greatest.h>
#include "listIden.h"

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

MemorySlot initTestMemory()
{
    MemorySlot space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))
    space->used = false;
    space->offset = -1;
    space->next = NULL;
    // len(var_ + NUL char) = 5
    size_t len = strlen("test") + 5;
    CHECKPOINTER(space->label = malloc(len))
    snprintf(space->label, len, "var_%s", "test");

    return space;
}

//------------------------//
// initListIdentifierTest //
//------------------------//
TEST initListIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(0, addr->numberIdentifiers,"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

//------------------------//
// addIntoListIdentifier //
//------------------------//
TEST addIntoListIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addIntoListIdentifier(addr, name, initTestMemory());

    ASSERT_EQ_FMT(1, addr->numberIdentifiers,"%d");
    ASSERT_STR_EQ(name, addr->Identifiers[0]->name);

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST addIntoListIdentifierMaxSizeTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test100";

    //simule le remplissage de la liste
    int i;
    for(i = 0; i < IDEN_MAX; i++){
        addIntoListIdentifier(addr, names[i], initTestMemory());
    }

    ASSERT_EQ_FMT(IDEN_MAX, addr->numberIdentifiers,"%d");
    ASSERT_EQ_FMT(RETURN_FAILURE, addIntoListIdentifier(addr, name,initTestMemory()), "%d");
    ASSERT_EQ_FMT(NOTFOUND, searchIntoListIdentifier(addr,name),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}
TEST addIntoListIdentifierWithBadNameTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "";
    addIntoListIdentifier(addr, name, initTestMemory());

    ASSERT_EQ_FMT(RETURN_FAILURE, addIntoListIdentifier(addr, name,initTestMemory()), "%d");
    ASSERT_EQ_FMT(0, addr->numberIdentifiers,"%d");
    ASSERT_EQ_FMT(NOTFOUND, searchIntoListIdentifier(addr,name),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}


//--------------------------//
// searchIntoListIdentifier //
//--------------------------//
TEST searchIdentifierPositionWithoutIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";

    ASSERT_EQ_FMT(NOTFOUND, searchIntoListIdentifier(addr,name),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST searchIdentifierPositionWithIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addIntoListIdentifier(addr, name, initTestMemory());

    ASSERT_EQ_FMT(0, searchIntoListIdentifier(addr,name),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST searchIdentifierPositionWithAnOtherIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    char* name1 = "test1";
    addIntoListIdentifier(addr, name, initTestMemory());

    ASSERT_EQ_FMT(NOTFOUND, searchIntoListIdentifier(addr,name1),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

//---------------------//
// setTypeOfIdentifier //
//---------------------//
TEST setTypeWithoutIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    int position = searchIntoListIdentifier(addr,name);
    int type = INTEGER;

    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeOfIdentifier(addr,position,type),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST setTypeWithBadPositionTest(void) {
    ListIdentifier addr = initListIdentifier();
    int position = -2;
    int type = INTEGER;

    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeOfIdentifier(addr,position,type),"%d");
    position = IDEN_MAX;
    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeOfIdentifier(addr,position,type),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST setTypeWithBadTypeTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addIntoListIdentifier(addr, name, initTestMemory());

    int position = searchIntoListIdentifier(addr,name);
    int type = UNSET;

    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeOfIdentifier(addr,position,type),"%d");
    type = MAXTYPEVALUE;
    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeOfIdentifier(addr,position,type),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST setTypeWithGoodTypesTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addIntoListIdentifier(addr, name, initTestMemory());
    int position = searchIntoListIdentifier(addr,name);

    int type = CHAIN;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeOfIdentifier(addr,position,type),"%d");
    type = INTEGER;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeOfIdentifier(addr,position,type),"%d");
    type = ARRAY;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeOfIdentifier(addr,position,type),"%d");
    type = FUNCTION;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeOfIdentifier(addr,position,type),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

//--------------------------//
// setArraySizeOfIdentifier //
//--------------------------//
TEST setArraySizeWithoutIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    int position = searchIntoListIdentifier(addr,name);
    int arraySize = 1;

    ASSERT_EQ_FMT(RETURN_FAILURE, setArraySizeOfIdentifier(addr,position,arraySize),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST setArraySizeWithBadPositionTest(void) {
    ListIdentifier addr = initListIdentifier();
    int position = -2;
    int arraySize = 1;

    ASSERT_EQ_FMT(RETURN_FAILURE, setArraySizeOfIdentifier(addr,position,arraySize),"%d");
    position = IDEN_MAX;
    ASSERT_EQ_FMT(RETURN_FAILURE, setArraySizeOfIdentifier(addr,position,arraySize),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST setArraySizeWithBadArraySizeTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addIntoListIdentifier(addr, name, initTestMemory());

    int position = searchIntoListIdentifier(addr,name);
    int arraySize = 0;

    ASSERT_EQ_FMT(RETURN_FAILURE, setArraySizeOfIdentifier(addr,position,arraySize),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

TEST setArraySizeWithGoodArraySizeTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addIntoListIdentifier(addr, name, initTestMemory());
    int position = searchIntoListIdentifier(addr,name);

    int arraySize = 1;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setArraySizeOfIdentifier(addr,position,arraySize),"%d");
    arraySize = 10;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setArraySizeOfIdentifier(addr,position,arraySize),"%d");

    error = 0;
    cleanListIdentifier(addr);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    log_set_quiet(false); // No logs in tests
    listInstruction = initListInstruction();

    RUN_TEST(initListIdentifierTest);

    RUN_TEST(addIntoListIdentifierTest);
    RUN_TEST(addIntoListIdentifierMaxSizeTest);
    RUN_TEST(addIntoListIdentifierWithBadNameTest);

    RUN_TEST(searchIdentifierPositionWithoutIdentifierTest);
    RUN_TEST(searchIdentifierPositionWithIdentifierTest);
    RUN_TEST(searchIdentifierPositionWithAnOtherIdentifierTest);

    RUN_TEST(setTypeWithoutIdentifierTest);
    RUN_TEST(setTypeWithBadPositionTest);
    RUN_TEST(setTypeWithBadTypeTest);
    RUN_TEST(setTypeWithGoodTypesTest);

    RUN_TEST(setArraySizeWithoutIdentifierTest);
    RUN_TEST(setArraySizeWithBadPositionTest);
    RUN_TEST(setArraySizeWithBadArraySizeTest);
    RUN_TEST(setArraySizeWithGoodArraySizeTest);

    cleanListInstruction(listInstruction);

    GREATEST_MAIN_END();
}

