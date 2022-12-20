#!/bin/bash

echo "Cloning submodules..."
git submodule update --init --recursive

echo "Downloading mars to lib folder"
wget -P lib/ http://courses.missouristate.edu/KenVollmar/MARS/MARS_4_5_Aug2014/Mars4_5.jar

echo "Init CMake"
cmake . cmake-build-debug/

echo "Building CMake"
cmake --build cmake-build-debug/