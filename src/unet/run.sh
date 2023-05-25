#!/bin/bash

cd unet-pytorch-main
python export.py
cd ..
mkdir build
cd build
cmake ..
make -j32
cd ../res
./unet