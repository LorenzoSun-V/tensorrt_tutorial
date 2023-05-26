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

include_directories(
    ${include_dir}
    ${cuda_home}/include/cuda
	${cuda_home}/include/tensorRT
	${cpp_pkg}/opencv4.2/include
    ${cpp_pkg}/onnxruntime-linux-x64-1.10.0/include
    ${cpp_pkg}/openvino_2022.1.0.643/runtime/include
    ${cpp_pkg}/openvino_2022.1.0.643/runtime/include/ie
	${cuda_home}/include/protobuf
)

link_directories(
    ${cpp_pkg}/opencv4.2/lib
    ${cpp_pkg}/onnxruntime-linux-x64-1.10.0/lib
    ${cpp_pkg}/openvino_2022.1.0.643/runtime/lib/intel64
    ${cpp_pkg}/openvino_2022.1.0.643/runtime/3rdparty/tbb/lib
    ${cuda_home}/lib64
    ${syslib}
)

file(GLOB onnxtensorrt ${include_dir}/onnx-tensorrt/*.cpp)
file(GLOB onnx ${include_dir}/onnx/*.cpp)
add_executable(yolov5 ${PROJECT_SOURCE_DIR}/yolov5.cpp ${onnxtensorrt} ${onnx})
add_executable(yolov5_ort ${PROJECT_SOURCE_DIR}/yolov5_ort.cpp ${onnxtensorrt} ${onnx})
add_executable(yolov5_openvino ${PROJECT_SOURCE_DIR}/yolov5_openvino.cpp ${onnxtensorrt} ${onnx})

set(all_library 
    opencv_core
    opencv_imgproc 
    opencv_imgcodecs
    cudart 
    cudnn
    nvinfer 
    nvinfer_plugin
    protobuf
    onnxruntime
    openvino_onnx_frontend
    openvino
    tbb
)

target_link_libraries(yolov5 ${all_library})
target_link_libraries(yolov5_ort ${all_library})
target_link_libraries(yolov5_openvino ${all_library})