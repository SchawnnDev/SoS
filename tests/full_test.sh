#!/bin/bash

MARS_BIN_FILE="../lib/Mars4_5.jar"
CMAKE_BUILD_FOLDER="../cmake-build-debug/"
EXEC_FILE="${CMAKE_BUILD_FOLDER}sos"

# Colors
RED='\033[0;31m'
YELLOW='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Creates a temporary file
temp_asm_file=$(mktemp)

# ----------------
# Loops of testing
# ----------------

# Concatenation
for f in $(ls ./runtime/concatenation)
do
  test_name=$(echo $f | sed -e 's/.sos*//')

  $EXEC_FILE "runtime/concatenation/${test_name}.sos" -o "$temp_asm_file"
  rcode=$?
  # Checking if a unexpected error occurred
  if [ ! $rcode == 0 ]; then
    echo -e "${RED}Compiler failure (exit code $rcode).${NC}"
    rm "${temp_asm_file}"
    exit $rcode
  fi

  # Getting rid of the header with Mips information and the last line
  java -jar $MARS_BIN_FILE "$temp_asm_file" | tail -n +3 1> "./tmp/${test_name}_result"

  # Checking the validity of the result
  diff "tmp/${test_name}_result" "expected_result/concatenation/${test_name}_result" &>/dev/null
  if [ $? == 0 ];
  then
    echo -e "${GREEN}[V] Test ${test_name} successfully passed !${NC}"
  else
    echo -e "${RED}[X] Test ${test_name} failed.${NC}"
    echo -e "Expected:"
    cat "expected_result/concatenation/${test_name}_result"
    echo -e "Got:"
    cat "tmp/${test_name}_result"
  fi
done

# Clean
rm "${temp_asm_file}"

exit 0

#rm ./tmp/*