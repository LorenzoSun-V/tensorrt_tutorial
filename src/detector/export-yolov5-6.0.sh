#!/bin/bash

cd yolov5-6.0
python export.py --weights=../res/yolov5s.pt --dynamic --include=onnx --opset=11