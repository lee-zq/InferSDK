import onnxruntime as ort
import numpy as np
import cv2
import os
import sys
import time
import onnx

class InferAgent:
    def __init__(self, model_path):
        self.sess = ort.InferenceSession(model_path)
        self.input_name = self.sess.get_inputs()[0].name
        self.output_name = self.sess.get_outputs()[0].name

    def inference(self, in_data):
        pred = self.sess.run([self.output_name], {self.input_name: in_data})
        return pred

    def preprocess(self, img):
        pass

    def postprocess(self, data):
        pass