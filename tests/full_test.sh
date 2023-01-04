#!/bin/bash

MARS_BIN_FILE=lib/Mars4_5.jar
CMAKE_BUILD_FOLDER=cmake-build-debug/
EXEC_FILE="${CMAKE_BUILD_FOLDER}sos"

# Colors
RED='\033[0;31m'
YELLOW='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Creates a temporary file
temp_asm_file=$(mktemp)

# ------------------
# Test loop function
# ------------------
test_loop()
{
  test_type=$1

  for f in $(ls tests/runtime/${test_type})
  do
    test_name=$(echo $f | sed -e 's/.sos*//')

    $EXEC_FILE "tests/runtime/${test_type}/${test_name}.sos" -o "$temp_asm_file"
    rcode=$?
    # Checking if a unexpected error occurred
    if [ ! $rcode == 0 ]; then
      echo -e "${RED}Compiler failure (exit code $rcode).${NC}"
      rm "${temp_asm_file}"
      exit $rcode
    fi

    # Getting rid of the header with Mips information and the last line
    java -jar $MARS_BIN_FILE "$temp_asm_file" | tail -n +3 1> "tests/tmp/${test_name}_result"

    # Checking the validity of the result
    diff "tests/tmp/${test_name}_result" "tests/expected_result/${test_type}/${test_name}_result" &>/dev/null
    if [ $? == 0 ];
    then
      echo -e "${GREEN}[V] Test ${test_name} successfully passed !${NC}"
    else
      echo -e "${RED}[X] Test ${test_name} failed.${NC}"
      echo -e "Expected:"
      cat "tests/expected_result/${test_type}/${test_name}_result"
      echo -e "Got:"
      cat "tests/tmp/${test_name}_result"
    fi
  done
}

compilation_error_tests()
{
  test_type=$1

  for f in $(ls tests/compilation/${test_type})
  do
    test_name=$(echo $f | sed -e 's/.sos*//')

    $EXEC_FILE "tests/runtime/${test_type}/${test_name}.sos" -o "$temp_asm_file"
    rcode=$?
    # Checking that it returns an error
    if [ ! $rcode == 0 ]; then
      echo -e "${GREEN}[V] Error test ${test_name} successfully passed ! (Returned a compiler failure of code $rcode)${NC}"
      echo -e "${RED}Compiler failure (exit code $rcode).${NC}"
    else
      echo -e "${RED}[X] Test ${test_name} failed. It shouldn't compile.${NC}"
    fi
  done
}


# ------
# Tests
# ------
# Compilation
#compilation_error_tests
# Concatenation
test_loop "concatenation"
# Expression
#test_loop "expression"

# Clean
rm "${temp_asm_file}"

rm tests/tmp/*

exit 0

