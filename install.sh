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

if [ $(dpkg-query -W -f='${Status}' flex 2>/dev/null | grep -c "Flex is installed... skipping to next step.") -eq 0 ];
then
  echo "Installing flex using apt..."
  apt-get -y install flex;
fi

if [ $(dpkg-query -W -f='${Status}' bison 2>/dev/null | grep -c "Bison is installed... skipping to next step.") -eq 0 ];
then
  echo "Installing bison using apt..."
  apt-get -y install bison;
fi

if [ ! -f $CMAKE_BUILD_FOLDER ]; then
	echo "Creating folder ${CMAKE_BUILD_FOLDER}..."
	mkdir $CMAKE_BUILD_FOLDER
	cd $CMAKE_BUILD_FOLDER
	
	echo "Starting CMake init..."
	cmake ..
	
	echo "Building CMake..."
	cmake --build .
	
	echo "Building using makefile..."
	make
else
	echo "Folder ${CMAKE_BUILD_FOLDER} already exists... skipping CMake init!"
fi;