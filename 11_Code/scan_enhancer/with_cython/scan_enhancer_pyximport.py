import pyximport

pyximport.install()

from PIL import Image
import numpy as np
from scipy.ndimage import gaussian_filter
#pyx importieren
from filter import custom_filter
import time


def rgb2gray(rgb_img):
    # return np.asarray(rgb_img, dtype=np.double)
    return np.dot(rgb_img, [0.2989, 0.5870, 0.1140])


def enhance_gray_img(gray_img):
    gray_img_copy = np.copy(gray_img)
    blurred_img = gaussian_filter(img, sigma=30)
    start_time = time.time()

    custom_filter(blurred_img, gray_img_copy)
    run_time = time.time() - start_time
    print("custom_filter", "in", "%.6f" % run_time, "seconds\n")
    return gray_img_copy


input_img = 'in.jpg'
output_img = 'out.png'

img = Image.open(input_img)
img = rgb2gray(np.array(img))
img = enhance_gray_img(img)
I8 = img.astype(np.uint8)
img = Image.fromarray(I8)
img.save(output_img)
