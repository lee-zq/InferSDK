import libs.common as common
import onnx
import numpy as np
import cv2, os, sys, tqdm, time
import onnxruntime as rt

import onnxruntime as ort
import numpy as np
import cv2
import os
import sys
import time
import onnx

class InferAgent:
    def __init__(self, model_path, classes_map=None):
        self.sess = ort.InferenceSession(model_path)
        self.input_name = self.sess.get_inputs()[0].name
        self.output_name = self.sess.get_outputs()[0].name
        self.classes_map = classes_map
        if self.classes_map:
            self.classes_map = common.load_classes_file(self.classes_map)
    def inference(self, in_data):
        pred = self.sess.run([self.output_name], {self.input_name: in_data})
        return pred

    def preprocess(self, img):
        mean_vec = np.array([0.485, 0.456, 0.406])
        stddev_vec = np.array([0.229, 0.224, 0.225])
        in_data = common.center_crop(img, 224, 224)
        in_data = (in_data/255. - mean_vec)/stddev_vec
        in_data = np.expand_dims(in_data.transpose(2, 0, 1), axis=0).astype(np.float32)
        return in_data

    def postprocess(self, pred_data):
        pred = common.softmax(np.array(pred_data).squeeze())
        max_idx = np.argmax(pred)
        return max_idx, pred[max_idx]

    def run(self, img_path):
        img = cv2.imread(img_path)
        T1 = time.time()
        in_data = self.preprocess(img)
        pred = self.inference(in_data)
        result = self.postprocess(pred)
        T2 = time.time()
        print(f"inference time: {(T2-T1)*1000:.2f}ms")
        return result
    
    def run_multi(self, img_dir):
        img_lst = os.listdir(img_dir)
        img_paths = [os.path.join(img_dir, filename) for filename in img_lst]
        results = []
        for img_path in img_paths:
            result = self.run(img_path)
            results.append(result)
            print(f"img_path: {img_path} pred: id:{result[0]} score:{result[1]}")
            if self.classes_map:
                print(f"pred class: {self.classes_map[result[0]]}\n")
        return results
    
def mobilenet_v2_model_1000class_test():
    model_path = "output/res/mobilenetv2-12/mobilenetv2-12-int8.onnx"
    img_dir = "output/data/voc_data"
    class_map_file = "demo/python/class_id_1000.txt"
    agent = InferAgent(model_path, class_map_file)
    agent.run_multi(img_dir)
    
if __name__ == "__main__":
    mobilenet_v2_model_1000class_test()