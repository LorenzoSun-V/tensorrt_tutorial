#!/bin/bash

python gen-onnx.py
mkdir build
cd build
cmake ..
make -j32
cd ../res
./classifier