#!/bin/bash

build_type="Debug"
refresh=false
rebuild=false
run=false

on_build_type () {
    if echo "$1" | grep -qi "debug"; then
        build_type="Debug"
    elif echo "$1" | grep -qi "release"; then
        build_type="Release"
    elif [ "$1" == "-d" ]; then
        build_type="Debug"
    elif [ "$1" == "-r" ]; then
        build_type="Release"
    fi
}

handle_other_args () {
    if echo "$1" | grep -qi "refresh"; then
        refresh=true
    fi

    if echo "$1" | grep -qi "rebuild"; then
        rebuild=true
    fi

    if echo "$1" | grep -qi "run"; then
        run=true
    fi
}

for arg in "$@"
do
    on_build_type "$arg"
    handle_other_args "$arg"
done

if [ "$rebuild" = true ]; then
    rm -rf build 
    rm -rf bin
fi

if [ ! -d ./build ] || [ "$refresh" = true ]; then
    ./cmake_generate.sh "$build_type"
fi 

if cmake --build build -j "$(nproc)"; then
    ./copy_res.sh

    if [ $run = true ]; then
        echo "Running with build type $build_type"
        cd ./bin
        ./schism
        cd ..
    fi

else
    echo BUILD FAILED    
fi

