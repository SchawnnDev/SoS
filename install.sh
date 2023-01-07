#!/bin/bash

LIB_DIR=lib/
CMAKE_BUILD_FOLDER=cmake-build-debug/
MARS_BIN_NAME=Mars4_5.jar

# Colors
RED='\033[0;31m'
YELLOW='\033[0;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Starting installation!${NC}"
echo ""

echo -e "${YELLOW}Updating submodules...${NC}"
git submodule update --init --recursive

if [ ! -f $LIB_DIR$MARS_BIN_NAME ]; then
  echo -e "${YELLOW}Downloading mars to lib folder${NC}"
  wget -P $LIB_DIR http://courses.missouristate.edu/KenVollmar/MARS/MARS_4_5_Aug2014/$MARS_BIN_NAME
else
  echo -e "${YELLOW}Mars already downloaded! Skipping to next step.${NC}"
fi

if [ "$EUID" -ne 0 ]; then
  echo -e "${RED}Skipping cmake, flex, bison & java installation... Script must be run as root.${NC}"
  echo -e "${RED}If cmake, flex, bison or java are missing, the CMake build will probably crash.${NC}"
  echo -e "${RED}You can cancel this installation and rerun the script with root permissions. Waiting 3 seconds.${NC}"
  sleep 3
else
  if [ $(dpkg-query -W -f='${Status}' flex 2>/dev/null | grep -c "install ok installed") -eq 0 ]; then
    echo -e "${YELLOW}Installing flex using apt...${NC}"
    apt-get -y install flex
  else
    echo -e "${YELLOW}Flex is installed... skipping to next step.${NC}"
  fi

  if [ $(dpkg-query -W -f='${Status}' bison 2>/dev/null | grep -c "install ok installed") -eq 0 ]; then
    echo -e "${YELLOW}Installing bison using apt...${NC}"
    apt-get -y install bison
  else
    echo -e "${YELLOW}Bison is installed... skipping to next step.${NC}"
  fi

  if [ $(dpkg-query -W -f='${Status}' java 2>/dev/null | grep -c "install ok installed") -eq 0 ]; then
    echo -e "${YELLOW}Installing java using apt...${NC}"
    apt-get -y install openjdk-8-jre
  else
    echo -e "${YELLOW}Java is installed... skipping to next step.${NC}"
  fi

  if [ $(dpkg-query -W -f='${Status}' cmake 2>/dev/null | grep -c "install ok installed") -eq 0 ]; then
    echo -e "${YELLOW}Installing cmake using apt...${NC}"
    apt-get -y install cmake
  else
    echo -e "${YELLOW}CMake is installed... skipping to next step.${NC}"
  fi
fi

if [ ! -d $CMAKE_BUILD_FOLDER ]; then
  echo -e "${YELLOW}Creating folder ${CMAKE_BUILD_FOLDER}...${NC}"
  mkdir $CMAKE_BUILD_FOLDER
  cd $CMAKE_BUILD_FOLDER || (echo "${RED}Can't access to folder ${CMAKE_BUILD_FOLDER}, exiting...${NC}" && exit)

  echo -e "${YELLOW}Starting CMake init...${NC}"
  cmake ..

  echo -e "${YELLOW}Building CMake...${NC}"
  cmake --build . --target sos

  echo -e "${YELLOW}Building using makefile...${NC}"
  make
else
  echo -e "${YELLOW}Folder ${CMAKE_BUILD_FOLDER} already exists... skipping CMake init!${NC}"
fi

echo ""
echo -e "${GREEN}Installation finished!${NC}"
