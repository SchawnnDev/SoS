#!/bin/bash

MARS_BIN_FILE=lib/Mars4_5.jar
CMAKE_BUILD_FOLDER=cmake-build-debug/
EXEC_FILE="${CMAKE_BUILD_FOLDER}sos"

# Colors
RED='\033[0;31m'
YELLOW='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

helpFunction() {
  echo -e "${YELLOW}Help for the script $0:"
  echo -e "Usage: $0 <param>"
  echo -e "\tshort: Starts short tests"
  echo -e "\tfull: Starts full tests set"
  echo -e "\tfile <file_path>: Testing an SoS file directly${NC}"
  exit 1
}

if [ $# -eq 0 ]; then
  helpFunction
fi

if [ ! -f $MARS_BIN_FILE ]; then
  echo -e "${RED}Missing ${MARS_BIN_FILE}... Please use install.sh to download it.${NC}"
  exit 1
fi

if [ ! -d $CMAKE_BUILD_FOLDER ]; then
  echo -e "${RED}Missing ${CMAKE_BUILD_FOLDER}... Please use install.sh to create it.${NC}"
  exit 1
fi

echo -e "${YELLOW}Building binary in ${CMAKE_BUILD_FOLDER}...${NC}"
cd $CMAKE_BUILD_FOLDER || (echo "${RED}Can't access to folder ${CMAKE_BUILD_FOLDER}, exiting...${NC}" && exit)
make
rcode=$?
cd ..

  if [ ! $rcode == 0 ]; then
    echo -e "${RED}Make failure (exit code $rcode).${NC}"
    exit $rcode
  fi

# TODO: Steps :
#   - Get sos file
#   - Compile it to an tmp file
#   - Test it with Mars
#   - Compare output
if [ "$1" == "file" ]; then

  if [ -z "$2" ]; then
    echo -e "${RED}Missing args: $0 $1 <file_path>${NC}"
    exit 1
  fi

  if [ ! -f "$2" ]; then
    echo -e "${RED}File $2 not exists.${NC}"
    exit 1
  fi

  # Creates a temporary file
  temp_asm_file=$(mktemp)

  $EXEC_FILE "$2" -v 1 -o "$temp_asm_file"
  rcode=$?
  if [ ! $rcode == 0 ]; then
    echo -e "${RED}Compiler failure (exit code $rcode).${NC}"
    rm "${temp_asm_file}"
    exit $rcode
  fi

  # temp :
  java -jar $MARS_BIN_FILE "$temp_asm_file"

  # Clean
  rm "${temp_asm_file}"
fi