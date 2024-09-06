#!/bin/bash

if [ $# -gt 0 ]; then
    if echo "$1" | grep -qi "debug"; then
        build_type="Debug"
    elif echo "$1" | grep -qi "release"; then
        build_type="Release"
    elif [ "$1" == "--d" ]; then
        build_type="Debug"
    elif [ "$1" == "--r" ]; then
        build_type="Release"
    else
        build_type="Debug"
    fi
else
    build_type="Debug"
fi

# add cmake presets in the future
if [ "$build_type" == "Debug" ]; then
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE="$build_type" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DENABLE_SANITIZER=1 -DUSE_CPPCHECK=0
    cp ./build/compile_commands.json ./
else
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE="$build_type"
fi


