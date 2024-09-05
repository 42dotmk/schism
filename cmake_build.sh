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

if [ ! -d ./build ]; then
    ./cmake_generate.sh "$build_type"
fi 

if cmake --build build -j $(nproc); then
    ./copy_res.sh
    cd bin
    ./schism
    cd ..
else
    echo BUILD FAILED    
fi
