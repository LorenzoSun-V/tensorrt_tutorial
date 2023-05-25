# Classifier的使用

## Step 1. 安装依赖

依赖安装完成后，可直接进入`classifier`目录，执行`run.sh`生成onnx、编译、运行程序。

## Step 2. 生成onnx文件

```shell
cd src/classifier
python gen-onnx.py -r resnet18
```

Tips: 默认是resnet18模型。

## Step 3. 编译

```shell
cd src/classifier
mkdir build
cd build
cmake ..
make -j4
```

## Step 4. 运行

```shell
cd src/classifier/res
./classifier
```