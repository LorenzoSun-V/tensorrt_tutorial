cmake_minimum_required(VERSION 3.16)

project(tensorrt_tutorial)
set(include_dir ../../include)
set(cuda_home ${include_dir}/trt8cuda115cudnn8)
set(cpp_pkg ${include_dir}/cpp-packages)
set(syslib ../../lib)
set(EXECUTABLE_OUTPUT_PATH ../res)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(ENV{LD_LIBRARY_PATH} /ssd1T/clearning/tensorrt/tensorrt_tutorial/include/cpp-packages/opencv4.2/lib)

include_directories(
    ${include_dir}
    ${cuda_home}/include/cuda
	${cuda_home}/include/tensorRT
	${cpp_pkg}/opencv4.2/include
	${cuda_home}/include/protobuf
)

link_directories(
    ${cpp_pkg}/opencv4.2/lib
    ${cuda_home}/lib64
    ${syslib}
)

file(GLOB onnxtensorrt ${include_dir}/onnx-tensorrt/*.cpp)
file(GLOB onnx ${include_dir}/onnx/*.cpp)
file(GLOB mlinfer ./*.cpp)
add_executable(multithread ${PROJECT_SOURCE_DIR}/multithread.cpp ${onnxtensorrt} ${onnx} ${mlinfer})

set(all_library 
    opencv_core
    opencv_imgproc 
    opencv_imgcodecs
    cudart 
    cudnn
    nvinfer 
    nvinfer_plugin
    protobuf
)

target_link_libraries(multithread ${all_library})


