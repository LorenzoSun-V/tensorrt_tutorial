#!/bin/bash

cd yolov5-6.0

python detect.py --weights=../res/yolov5s.pt --source=../res/car.jpg --iou-thres=0.5 --conf-thres=0.25 --project=../res/

mv ../res/exp/car.jpg ../res/car-pytorch.jpg
rm -rf ../res/exp