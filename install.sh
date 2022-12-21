#!/bin/bash

LIB_DIR=lib/
CMAKE_BUILD_FOLDER=cmake-build-debug/
MARS_BIN_NAME=Mars4_5.jar

echo "Updating submodules..."
git submodule update --init --recursive

if [ ! -f $LIB_DIR$MARS_BIN_NAME ]; then
  echo "Downloading mars to lib folder"
  wget -P $LIB_DIR http://courses.missouristate.edu/KenVollmar/MARS/MARS_4_5_Aug2014/$MARS_BIN_NAME
else
  echo "Mars already downloaded! Skipping to next step."
fi;

echo "Init CMake"
cmake $CMAKE_BUILD_FOLDER

echo "Building CMake"
cmake --build $CMAKE_BUILD_FOLDER