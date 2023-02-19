from PIL import Image
import numpy as np
from scipy.ndimage import gaussian_filter


def rgb2gray(rgb_img):
    return np.dot(rgb_img, [0.2989, 0.5870, 0.1140])


def custom_filter(blurred_img, gray_img):
    for i in range(gray_img.shape[0]):
        for j in range(gray_img.shape[1]):
            d = gray_img[i][j] / (blurred_img[i][j] + 0.00000000001)
            m = min(255, d * 255 + 0.00000000001)
            gamma = 0.5 * (m / 255.0) * m * m / 65025
            m = ((m / 255.0) ** (1 / (gamma + 0.00000000001))) * 255.0
            gray_img[i][j] = m


def enhance_gray_img(gray_img):
    gray_img_copy = np.copy(gray_img)
    blurred_img = gaussian_filter(img, sigma=30)

    print(blurred_img.dtype)
    print(blurred_img.shape)
    print(gray_img_copy.dtype)
    print(gray_img_copy.shape)

    custom_filter(blurred_img, gray_img_copy)
    return gray_img_copy


input_img = 'in.jpg'
output_img = 'out.png'

img = Image.open(input_img)
img = rgb2gray(np.array(img))
img = enhance_gray_img(img)
I8 = img.astype(np.uint8)
img = Image.fromarray(I8)
img.save(output_img)
