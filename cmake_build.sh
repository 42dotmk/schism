#!/bin/bash

if cmake --build build -j $(nproc); then
    ./copy_res.sh
    cd bin
    ./schism
    cd ..
else
    echo BUILD FAILED    
fi
