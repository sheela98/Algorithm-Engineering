# Enhancer for scanned images

### Algorithm-Engineering

## Description

The repository includes:

- Implementation and paper for the image enhancer
- Exam Assignments
- Coding warmups (1 - 4) in the respective folders

This implementation presents an image enhancer as part of a university project written in C++. It takes a PPM image as input and uses several techniques of image processing to improve the quality of the image. The project is described in the paper included in the repository including an in-depth description of the algorithms used.

The implementation applies a median filter to the image used for noise removal.
Furthermore, an adaptive thresholding method is applied for image segmentation.
The project makes use of several optimization techniques to improve performance.

---

## Keywords

- PPM, PMG image

- Image segmentation
- Median filter

- Adaptive binary thresholding

- Optimization
- OpenMP parallel, parallel for

---

## Instructions

input: ppm image
output: ppm image with improved quality

The implementation includes a CMake file to build the project.
This includes a folder with sample images to test the image enhancer.
A console application is provided, which takes PPM images as input.
The input and desired output path can be entered through the console.

Requirements:

- OpenMP is used for performance optimization and has to be installed.
  (Installation guide - https://www.geeksforgeeks.org/openmp-introduction-with-installation-guide/)
