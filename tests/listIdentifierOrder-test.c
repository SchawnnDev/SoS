#include <greatest.h>
#include "listRangeVar.h"

//-------------------//
// initRangeVariable //
//-------------------//
TEST initIdentifierOrderZeroTest(void) {
    IdentifierOrder addr = initIdentifierOrder(NULL, "test");

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->previousIdentifier);
    ASSERT_STR_EQ("test", addr->name);
    ASSERT_EQ_FMT(UNSET, addr->type,"%d");
    PASS();
}

//--------------------------//
// cleanIdentifierOrder//
//--------------------------//
TEST cleanIdentifierOrderTest(void) {
    IdentifierOrder addr = initIdentifierOrder(NULL,"test");
    cleanIdentifierOrder(addr);

    PASS();
}

//-------------------------//
// initListIdentifierOrder //
//-------------------------//
TEST initListIdentifierOrderTest(void) {
    ListIdentifierOrder addr = initListIdentifierOrder();

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ(NULL, addr->cursor);
    PASS();
}

//--------------------------//
// cleanListIdentifierOrder //
//--------------------------//
TEST cleanListIdentifierOrderTest(void) {
    ListIdentifierOrder addr = initListIdentifierOrder();
    addIdentifierOrder(addr,"test1");
    cleanListIdentifierOrder(addr);

    PASS();
}

//--------------------//
// addIdentifierOrder //
//--------------------//
TEST addIdentifierOrderTest(void) {
    ListIdentifierOrder addr = initListIdentifierOrder();
    addIdentifierOrder(addr,"test");

    ASSERT_NEQ(NULL, addr);
    ASSERT_NEQ(NULL, addr->cursor);
    ASSERT_STR_EQ("test", addr->cursor->name);
    ASSERT_EQ_FMT(UNSET, addr->cursor->type,"%d");
    PASS();
}

//------------------------//
// setTypeIdentifierOrder //
//------------------------//
TEST setTypeIdentifierOrderWithBadTypeTest(void) {
    ListIdentifierOrder addr = initListIdentifierOrder();
    addIdentifierOrder(addr, "test");

    int type = UNSET;
    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeIdentifierOrder(addr,type),"%d");
    type = MAXTYPEVALUE;
    ASSERT_EQ_FMT(RETURN_FAILURE, setTypeIdentifierOrder(addr,type),"%d");
    PASS();
}

TEST setTypeIdentifierOrderWithGoodTypesTest(void) {
    ListIdentifierOrder addr = initListIdentifierOrder();
    addIdentifierOrder(addr, "test");

    int type = CHAIN;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeIdentifierOrder(addr,type),"%d");
    type = INTEGER;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeIdentifierOrder(addr,type),"%d");
    type = ARRAY;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeIdentifierOrder(addr,type),"%d");
    type = FUNCTION;
    ASSERT_EQ_FMT(RETURN_SUCCESS, setTypeIdentifierOrder(addr,type),"%d");
    PASS();
}

//-----------------------//
// deleteIdentifierOrder //
//-----------------------//
TEST deleteIdentifierOrderTest(void) {
    ListIdentifierOrder addr = initListIdentifierOrder();

    ASSERT_EQ_FMT(RETURN_FAILURE, deleteIdentifierOrder(addr),"%d");
    addIdentifierOrder(addr, "test");
    ASSERT_EQ_FMT(RETURN_SUCCESS, deleteIdentifierOrder(addr),"%d");
    ASSERT_EQ(NULL, addr->cursor);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    log_set_quiet(true); // No logs in tests

    RUN_TEST(initIdentifierOrderZeroTest);

    RUN_TEST(cleanIdentifierOrderTest);

    RUN_TEST(initListIdentifierOrderTest);
    RUN_TEST(cleanListIdentifierOrderTest);
    RUN_TEST(addIdentifierOrderTest);

    RUN_TEST(setTypeIdentifierOrderWithBadTypeTest);
    RUN_TEST(setTypeIdentifierOrderWithGoodTypesTest);

    RUN_TEST(deleteIdentifierOrderTest);

    GREATEST_MAIN_END();
}

