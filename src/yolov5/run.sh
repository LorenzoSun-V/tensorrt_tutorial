#!/bin/bash

bash export-yolov5-6.0.sh
bash detect-for-yolov5-6.0.sh

mkdir build
cd build
cmake ..
make -j32
cd ../res
./yolov5