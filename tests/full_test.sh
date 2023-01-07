#!/bin/bash

MARS_BIN_FILE=lib/Mars4_5.jar
CMAKE_BUILD_FOLDER=cmake-build-debug/
EXEC_FILE="${CMAKE_BUILD_FOLDER}sos"

# Colors
RED='\033[0;31m'
YELLOW='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
BOLD=$(tput bold)

# Counters
passed=0
failed=0

# Creates a temporary file
temp_asm_file=$(mktemp)

# Creating temp directory
mkdir tests/tmp

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
      passed=$((passed+1))
    else
      echo -e "${RED}[X] Test ${test_name} failed.${NC}"
      echo -e "Expected:"
      cat "tests/expected_result/${test_type}/${test_name}_result"
      echo -e "Got:"
      cat "tests/tmp/${test_name}_result"
      failed=$((failed+1))
    fi
  done
}

compilation_error_tests()
{
  for f in $(ls tests/compilation/${test_type})
  do
    test_name=$(echo $f | sed -e 's/.sos*//')

    $EXEC_FILE "tests/compilation/${test_name}.sos" -o "$temp_asm_file" &>/dev/null
    rcode=$?
    # Checking that it returns an error
    if [ ! $rcode == 0 ]; then
      echo -e "${GREEN}[V] Error test ${test_name} successfully passed ! (Returned a compiler failure of code $rcode)${NC}"
      passed=$((passed+1))
    else
      echo -e "${RED}[X] Test ${test_name} failed. It shouldn't compile.${NC}"
      failed=$((failed+1))
    fi
  done
}


# ------
# Tests
# ------
# Compilation
echo "${BOLD}-- Compilation tests --"
compilation_error_tests
# Assign
echo "${BOLD}-- Assignment tests --"
test_loop "assign"
# Concatenation
echo "${BOLD}-- Concatenation tests --"
test_loop "concatenation"
# Expression
echo "${BOLD}-- Expression tests --"
test_loop "expression"
# General test_block
echo "${BOLD}-- General test block tests --"
test_loop "general_test_block"
# Relational test_block
echo "${BOLD}-- Relational test block tests --"
test_loop "relational_test_block"
# Conditional test_block
echo "${BOLD}-- Conditional test block tests --"
test_loop "conditional_test_block"
# If tests
echo "${BOLD}-- If tests --"
test_loop "if"
# While tests
echo "${BOLD}-- While tests --"
test_loop "while"
# Until tests
echo "${BOLD}-- Until tests --"
test_loop "until"
# Fizzbuzz test
echo "${BOLD}-- Fizzbuzz tests --"
test_loop "fizzbuzz"
# Array test
echo "${BOLD}-- Array tests --"
test_loop "array"
# Function test
echo "${BOLD}-- Function tests --"
test_loop "function"

# Summary
echo -e "${BOLD}-- SUMMARY --${NC}"
echo -e "Total ${GREEN}passed${NC} test = ${passed}"
echo -e "Total ${RED}failed${NC} test = ${failed}"


# Clean
rm "${temp_asm_file}"

rm -rf tests/tmp/ &>/dev/null

exit 0

