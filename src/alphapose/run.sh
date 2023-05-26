#!/bin/bash

cd AlphaPose
export PYTHONPATH=.
python scripts/export.py
cd ..
mkdir build
cd build
cmake ..
make -j32
cd ../res
./alphapose