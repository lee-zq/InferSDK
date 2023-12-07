import numpy as np
import libs.common as common
import onnx
import cv2, os, sys, tqdm, time
import onnxruntime as rt
from matplotlib.colors import hsv_to_rgb
import onnxruntime as ort
import distinctipy

voc_classes = ['aeroplane', 'bicycle', 'bird', 'boat', 'bottle', 
            'bus', 'car', 'cat', 'chair', 'cow', 
            'diningtable', 'dog', 'horse','motorbike', 'person', 
            'pottedplant','sheep', 'sofa', 'train', 'tvmonitor']
num_classes = len(voc_classes)

def get_color_list(num, scale=1):
    colors = distinctipy.get_colors(num)
    np.set_printoptions(precision=3,suppress=True)
    colors = np.array(colors) * scale
    # print(repr(colors))
    return colors

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
        mean=[0.485, 0.456, 0.406]
        std=[0.229, 0.224, 0.225]
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        image_data = np.array(img, dtype='float32')
        image_data /= 255.
        image_data -= mean
        image_data /= std

        image_data = np.transpose(image_data, [2, 0, 1])
        image_data = np.expand_dims(image_data, 0)
        return [image_data]

    def postprocess(self, pred_data):
        seg_one_hot = pred_data[0][0]
        # get classification labels
        raw_labels = np.argmax(seg_one_hot, axis=0).astype(np.uint8)
        # comput confidence score
        confidence = float(np.max(seg_one_hot, axis=0).mean())
        return raw_labels, confidence

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
            # print(f"img_path: {img_path} pred: id:{result[0]} score:{result[1]}")
            # if self.classes_map:
            #     print(f"pred class: {self.classes_map[result[0]]}\n")
            self.visualization(img_path, result)
        return results
    
    def visualization(self, img_path, result):
        img = cv2.imread(img_path)
        img_name = os.path.basename(img_path)
        raw_labels, confidence = result
        # generate segmented image
        color_map = get_color_list(21, scale=255)
        result_img = np.zeros_like(img)
        for i in range(num_classes):
            if i == 0:
                result_img[raw_labels == i] = [0,0,0]
            else:
                result_img[raw_labels == i] = color_map[i]
        # generate blended image
        blended_img = cv2.addWeighted(img[:, :, ::-1], 0.5, result_img, 0.5, 0)
        cv2.imwrite(f"./{img_name}", blended_img)

def test():
    model_path = "output/models/fcn_resnet50_seg/fcn-resnet50-12-int8.onnx"
    img_dir = "output/data/my_data"
    class_map_file = None
    agent = InferAgent(model_path, class_map_file)
    agent.run_multi(img_dir)
    
if __name__ == "__main__":
    test()