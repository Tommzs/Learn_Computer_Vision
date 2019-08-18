import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
import argparse

def read_args():
    parser = argparse.ArgumentParser(description="""Find object in image by template""")
    parser.add_argument(
        "-i", "--image", required=True, help="Path to image", default=None
    )
    parser.add_argument(
        "-t", "--template", required=True, help="Path to template", default=None
    )
    args = vars(parser.parse_args())
    return args["image"], args["template"]

if __name__ == "__main__":
    im_path, tp_path = read_args()
    img = cv.imread(im_path, 0)
    tmp = cv.imread(tp_path, 0)
    plt.imshow(img)