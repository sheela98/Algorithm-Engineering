# Enhancer for scanned images

### Algorithm-Engineering

## Answer (What? Why? How?)

## Description

This application is a university project written in C++. It takes an image as input and uses several techniques of image processing to enhance visibility of the image.
The output is an image aswell which is stored in the respective folder.

The application provides a console interface, which allows to input the name of the image to be processed. PPM images are supported by the program. Other image types can easily be converted using the command: <br>
`convert original_image.jpg -compress none converted_image.ppm`

The application applies a median filter to the image. This is used for noise removal.
Furthermore, an adaptive thresholding method is applied for image segmentation. Otsu's method is used to compute a threshold and apply it to the image.

The project makes use of several optimization techniques to improve performance. OpenMP is used to parallelize certain sections of code as well as other things.

- University project
- Enhancer for scanned images
- input ppm, output ppm
- improve quality of scanned images
- terminal application
- C++

---

## Keywords

- ppm, pgm image

- Image segmentation
- Median filter

- Adaptive binary thresholding
- Otsu's method

- Optimization
- OpenMP parallel, parallel for

---

## Instructions

input: ppm image
output: ppm image with improved quality

- Add tests
- Performance tests
