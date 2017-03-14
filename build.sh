#!/bin/bash
source /opt/rh/devtoolset-2/enable
rm -f CMakeCache.txt 
rm -rf CMakeFiles
cmake ./
make clean
make
./buildnode.sh
rm -f CMakeCache.txt
rm -rf CMakeFiles
rm -rf Makefile
rm -rf cmake_install.cmake
