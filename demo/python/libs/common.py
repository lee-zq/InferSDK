import onnxruntime as rt
import numpy as np
import cv2
import os
import sys
import time
import onnx

def img_normailize(img_data, std, mean):
    img_data = img_data.astype(np.float32)
    img_data = (img_data - mean) / std
    return img_data

def img_resize(img_data, h, w, interpolation=cv2.INTER_LINEAR):
    img_data = cv2.resize(img_data, (w, h), interpolation=interpolation)
    return img_data

def center_crop(img_data, crop_h, crop_w):
    h, w, c = img_data.shape
    if crop_h > h or crop_w > w:
        raise Exception("crop size is larger than image size")
    start_x = w//2 - crop_w//2
    start_y = h//2 - crop_h//2
    return img_data[start_y:start_y+crop_h, start_x:start_x+crop_w, :]

def softmax(x):
    tmp = np.exp(x - np.max(x))  # x-max(x) avoid overflow
    softmax = tmp/(np.sum(tmp))
    return softmax

def load_classes_file(classes_file):
    with open(classes_file, "r") as f:
        lines = f.readlines()
    classes_map = {}
    for idx, line in enumerate(lines):
        classes_map[idx] = line.strip()
    return classes_map