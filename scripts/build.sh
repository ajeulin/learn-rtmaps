#! /bin/bash
# **********************************************************
#      2024 : Arthur JEULIN
#      Copyright (c) Coast Autonomous 
#      Created by Arthur JEULIN on 03/30/2024.
# **********************************************************
#
# Module : Build the project
#


if [ -d "build" ]; then
    # Le dossier build existe, le vider
    rm -rf build/*
else
    # Le dossier build n'existe pas, le créer
    mkdir build
fi

cmake -S . -B build
cmake --build build -- -j 12
# Command to build the test
GTEST_COLOR=1 ctest --test-dir build --output-on-failure -- -j 12
sudo cmake --install build 