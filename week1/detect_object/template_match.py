import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
import argparse

def match_template(image, template):
    w, h = template.shape[::-1]
    res = cv.matchTemplate(image,template,cv.TM_CCOEFF_NORMED)
    min_val, max_val, min_loc, max_loc = cv.minMaxLoc(res)
    top_left = max_loc
    bottom_right = (top_left[0] + w, top_left[1] + h)
    cv.rectangle(image,top_left, bottom_right, 0, 5)
    plt.imshow(image,cmap = 'gray')
    plt.show()


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
    match_template(img, tmp)