#!/bin/bash

if [ ! -d ./build ]; then
    ./cmake_generate.sh
fi 

if cmake --build build -j $(nproc); then
    ./copy_res.sh
    cd bin
    ./schism
    cd ..
else
    echo BUILD FAILED    
fi
