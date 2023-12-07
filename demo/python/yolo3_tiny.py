import numpy as np
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

# this function is from yolo3.utils.letterbox_image
def letterbox_image(image, size):
    ih, iw = image.shape[:2]
    w, h = size
    scale = min(w/iw, h/ih)
    nw = int(iw*scale)
    nh = int(ih*scale)
    
    image = cv2.resize(image, (nw,nh), interpolation=cv2.INTER_CUBIC)
    # 创建一个新的图像，宽高为size，颜色为(128,128,128)
    new_image = np.ones((size[1], size[0], 3), dtype=np.uint8) * 128
    # 将缩放后的图像放在新图像中心
    new_image[(h-nh)//2:(h-nh)//2+nh, (w-nw)//2:(w-nw)//2+nw, :] = image
    return new_image

class InferAgent:
    def __init__(self, model_path, classes_map=None):
        self.sess = ort.InferenceSession(model_path)
        self.input_name = [i.name for i in self.sess.get_inputs()]
        self.output_name = [i.name for i in self.sess.get_outputs()]
        self.classes_map = classes_map
        if self.classes_map:
            self.classes_map = common.load_classes_file(self.classes_map)
    def inference(self, in_data):
        in_map = {}
        for name,value in zip(self.input_name, in_data):
            in_map[name] = value
        pred = self.sess.run(None, in_map)
        return pred

    def preprocess(self, img):
        model_image_size = (416, 416)
        boxed_image = letterbox_image(img, (model_image_size))
        image_data = np.array(boxed_image, dtype='float32')
        image_data /= 255.
        image_data = np.transpose(image_data, [2, 0, 1])
        image_data = np.expand_dims(image_data, 0)
        image_size = np.array([img.shape[0], img.shape[1]], dtype=np.float32).reshape(1, 2)
        return image_data, image_size

    def postprocess(self, pred_data):
        boxes = pred_data[0]
        scores = pred_data[1]
        indices = pred_data[2]
        out_boxes, out_scores, out_classes = [], [], []
        for idx_ in indices[0]:
            out_classes.append(idx_[1])
            out_scores.append(scores[tuple(idx_)])
            idx_1 = (idx_[0], idx_[2])
            out_boxes.append(boxes[idx_1])
        return out_classes, out_scores, out_boxes

    def run(self, img_path):
        img = cv2.imread(img_path)
        T1 = time.time()
        in_data = self.preprocess(img)
        pred = self.inference(in_data)
        result = self.postprocess(pred)
        T2 = time.time()
        # print(f"inference time: {(T2-T1)*1000:.2f}ms")
        return result
    
    def run_multi(self, img_dir):
        img_lst = sorted(os.listdir(img_dir))
        img_paths = [os.path.join(img_dir, filename) for filename in img_lst]
        results = []
        for img_path in img_paths:
            result = self.run(img_path)
            results.append(result)
            print(f"img_path: {img_path} pred: id:{result[0]} score:{result[1]}")
            # if self.classes_map:
            #     print(f"pred class: {self.classes_map[result[0]]}\n")
            self.visualization(img_path, result)
        return results
    
    def visualization(self, img_path, result):
        img = cv2.imread(img_path)
        img_name = os.path.basename(img_path)
        out_classes, out_scores, out_boxes = result
        for class_id, score, bbox in zip(out_classes, out_scores, out_boxes):
            left, top, right, bottom = bbox.astype(np.int32)
            cv2.rectangle(img, (top,left), (bottom, right), (0, 255, 0), 2)
            if self.classes_map:
                cv2.putText(img, self.classes_map[class_id], (top,left), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        cv2.imwrite(f"./{img_name}", img)

def mobilenet_v2_model_1000class_test():
    model_path = "output/res/yolov3_tiny/tiny-yolov3-11.onnx"
    img_dir = "output/data/voc_data"
    class_map_file = "output/res/yolov3_tiny/coco_classes.txt"
    agent = InferAgent(model_path, class_map_file)
    agent.run_multi(img_dir)
    
if __name__ == "__main__":
    mobilenet_v2_model_1000class_test()