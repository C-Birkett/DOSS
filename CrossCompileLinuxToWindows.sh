#!/bin/bash

cmake -B build -D CMAKE_TOOLCHAIN_FILE="mingw_toolchain.cmake" &&
cmake --build build --target clean &&
cmake --build build
