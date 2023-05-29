#!/bin/bash

# 对于main.cpp直接用g++编译即可
g++ mt_basic.cpp -o res/mt_basic -pthread
./res/mt_basic