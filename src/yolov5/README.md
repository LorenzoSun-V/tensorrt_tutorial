# 关于OpenVINO
1. 步骤：
    - 通过model = core.compile_model(xx.onnx)编译模型
    - 通过iq = model.create_infer_request()创建推理请求
    - input = iq.get_input_tensor(0);获取输入的tensor
    - output = iq.get_output_tensor(0);获取输出的tensor
    - input.set_shape({input_batch, input_channel, input_height, input_width});配置输入大小，因为是动态batch，需要先设置大小，此时会分配空间
    - input_data_host = input.data<float>();获取输入指针，必须set shape后才能获取数据指针，否则会存储空间没分配而异常
    - 把图像预处理并储存到 input_data_host
    - iq.infer() 执行推理步骤
    - output_data_host = output.data<float>();通过output拿到推理后的输出
    - 对output data进行解码得到最后的输出框

## 知识点
1. yolov5的预处理部分，使用了仿射变换，请参照仿射变换原理
    - letterbox采用双线性插值对图像进行resize，并且使源图像和目标图像几何中心的对齐
        ![avatar](./warpaffine/step1.png)
    - 使用仿射变换实现letterbox的理由是
        - 1. 便于操作，得到变换矩阵即可
        - 2. 便于逆操作，实则是逆矩阵映射即可
        - 3. 便于cuda加速，cuda版本的加速已经在cuda系列中提到了warpaffine实现
            - 该加速可以允许warpaffine、normalize、除以255、减均值除以标准差、变换RB通道等等在一个核中实现，性能最好
2. 后处理部分，反算到图像坐标，实际上是乘以逆矩阵
    - 而由于逆矩阵实际上有效自由度是3，也就是d2i中只有3个数是不同的，其他都一样。也因此你看到的是d2i[0]、d2i[2]、d2i[5]在作用