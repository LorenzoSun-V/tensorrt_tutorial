import argparse
import torch
import torchvision
import cv2
import numpy as np
import sys
sys.path.append("../..") # 添加项目根目录到环境变量中, 以便正确导入引用
from utils import Registry
backbone = Registry("backbone")


def parse_args():
    parser = argparse.ArgumentParser(
        description='Cls model convert to onnx')
    parser.add_argument('--model-name', '-m', default='resnet18', type=str, help='model name')
    args = parser.parse_args()
    return args


@backbone.register_module()
class Classifier(torch.nn.Module):
    def __init__(self, model_name="resnet18"):
        super().__init__()
        #使用torchvision自带的与训练模型, 更多模型请参考:https://tensorvision.readthedocs.io/en/master/
        self.backbone = eval(f'torchvision.models.{model_name}(pretrained=True)')
        
    def forward(self, x):
        feature     = self.backbone(x)
        probability = torch.softmax(feature, dim=1)
        return probability
        

# 对每个通道进行归一化有助于模型的训练
imagenet_mean = [0.485, 0.456, 0.406]
imagenet_std  = [0.229, 0.224, 0.225]

image = cv2.imread("res/dog.jpg")
image = cv2.resize(image, (224, 224))            # resize
image = image[..., ::-1]                         # BGR -> RGB
image = image / 255.0
image = (image - imagenet_mean) / imagenet_std   # normalize
image = image.astype(np.float32)                 # float64 -> float32
image = image.transpose(2, 0, 1)                 # HWC -> CHW
image = np.ascontiguousarray(image)              # contiguous array memory
image = image[None, ...]                         # CHW -> 1CHW
image = torch.from_numpy(image)                  # numpy -> torch
model = Classifier(parse_args().model_name).eval()

with torch.no_grad():
    probability   = model(image)
    
predict_class = probability.argmax(dim=1).item()
confidence    = probability[0, predict_class]

labels = open("res/labels.imagenet.txt").readlines()
labels = [item.strip() for item in labels]

print(f"Predict: {predict_class}, {confidence}, {labels[predict_class]}")

dummy = torch.zeros(1, 3, 224, 224)
torch.onnx.export(
    model, (dummy,), "res/classifier.onnx", 
    input_names=["image"], 
    output_names=["prob"], 
    dynamic_axes={"image": {0: "batch"}, "prob": {0: "batch"}},
    opset_version=11
)