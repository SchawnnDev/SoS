//
// Created by Quentin on 17/12/2022.
//
#include <greatest.h>
#include "listTmp.h"


//------------------------//
// initListIdentifierTest //
//------------------------//
TEST initListTmpTest(void) {
    ListTmp addr = initListTmp();

    ASSERT_NEQ(NULL, addr);
    ASSERT_EQ_FMT(0, addr->numberValues,"%d");
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_TEST(initListTmpTest);

    GREATEST_MAIN_END();
}

