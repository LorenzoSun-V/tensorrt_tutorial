#!/bin/bash

cd AlphaPose
python scripts/export.py
cd ..
mkdir build
cd build
cmake ..
make -j32
cd ../res
./alphapose