#!/bin/bash

bash export-yolov5-6.0.sh
bash detect-for-yolov5-6.0.sh
python inference-with-onnxruntime.py

mkdir build
cd build
cmake ..
make -j32
cd ../res
./yolov5

./yolov5_ort

#* to solve the problem of "libtbb.so.2: cannot open shared object file: No such file or directory"
export LD_LIBRARY_PATH=../../../include/cpp-packages/openvino_2022.1.0.643/runtime/3rdparty/tbb/lib/:$LD_LIBRARY_PATH
./yolov5_openvino