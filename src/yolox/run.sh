#!/bin/bash

pip install mmcv==2.0.0
pip install terminaltables
pip install pycocotools
pip install shapely

cd mmdetection
wget -P ../res/ https://download.openmmlab.com/mmdetection/v2.0/yolox/yolox_tiny_8x8_300e_coco/yolox_tiny_8x8_300e_coco_20211124_171234-b4047906.pth
python export.py

cd ..
mkdir build
cd build
cmake ..
make -j32

cd ../res/
./yolox