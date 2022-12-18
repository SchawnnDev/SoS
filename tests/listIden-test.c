//
// Created by Quentin on 17/12/2022.
//
#include <greatest.h>
#include "../include/listIden.h"

TEST initListIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(0, addr->numberIdentifiers,"%d");
    PASS();
}

TEST addIntoListIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addr = addIntoListIdentifier(addr, name);

    ASSERT_EQ_FMT(1, addr->numberIdentifiers,"%d");
    ASSERT_STR_EQ(name, addr->Identifiers[0]->name);
    PASS();
}

TEST searchIdentifierPositionWithoutIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    addr = addIntoListIdentifier(addr, name);

    ASSERT_EQ_FMT(-1, searchIdentifierPosition(addr,name),"%d");
    PASS();
}

TEST searchIdentifierPositionWithIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";

    ASSERT_EQ_FMT(0, searchIdentifierPosition(addr,name),"%d");
    PASS();
}

TEST searchIdentifierPositionWithAnOtherIdentifierTest(void) {
    ListIdentifier addr = initListIdentifier();
    char* name = "test";
    char* name1 = "test1";
    addr = addIntoListIdentifier(addr, name);

    ASSERT_EQ_FMT(-1, searchIdentifierPosition(addr,name1),"%d");
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_TEST(initListIdentifierTest);

    RUN_TEST(addIntoListIdentifierTest);

    RUN_TEST(searchIdentifierPositionWithoutIdentifierTest);
    RUN_TEST(searchIdentifierPositionWithIdentifierTest);
    RUN_TEST(searchIdentifierPositionWithAnOtherIdentifierTest);

    GREATEST_MAIN_END();
}

