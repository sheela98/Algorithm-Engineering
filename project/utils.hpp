//
// Created by Sheela on 28/02/23.
//


#ifndef PROJECT_UTILS_HPP
#define PROJECT_UTILS_HPP

#include <string>
using namespace std;

/* PIXEL
 * To separate the RGB values.
 */
struct Pixel {
    unsigned char r, g, b;
};

/*
 * IMAGE ENHANCER
 * Applies median filtering and adaptive thresholding to the image and returns the output.
 * Parameters: input image, output image
 * Returns: void
 */
void image_enhancer(const char *input, const char *output);

/* MEDIAN FILTER
 * Computes the median filter of an input image and writes the processed image to an output image.
 * Parameters: image, width, height
 * Returns: void
 */
void median_filter(Pixel* image, int width, int height);

/* APPLY MEDIAN FILTER
 * Reads the ppm image, applies the median filter and writes the output to the respective file.
 * Parameters: input, output
 * Returns: void
 */
void apply_median_filter(const char *input, const char *output);

/* READ PPM
 * Reads the ppm image and writes the width, height and returns the image pixels.
 * Parameters: input, width (empty), height (empty)
 * Returns: image pixels
 */
Pixel* read_ppm(const char* filename, int& width, int& height);

/* WRITE PPM
 * Writes the ppm image to the respective file.
 * Parameters: output, image, width, height
 * Returns: void
 */
void write_ppm(const char* filename, Pixel* image, int width, int height);

/* ADAPTIVE THRESHOLDING
 * Does image processing on a given input image with adaptive thresholding
 * the processed image is written to a different file.
 * Parameters: input image, output image, block size, threshold offset
 * Returns: void
 */
void adaptive_thresholding(const char *input_image, const char *output_image, int blockSize, int threshold_offset);

/* APPLY ADAPTIVE THRESHOLDING
 * Applies adaptive thresholding to ppm image and writes result to output.
 * Parameters: input image, output image
 * Returns: void
 */
void apply_adaptive_thresholding(const char *input, const char *output);

/* CONVERSION PPM TO PGM
 * Converts a ppm image to a pgm greyscale image:
 * Parameters: input image, output image
 * Returns: 1 for error, 0 if successful
*/
int convert_ppm_to_pgm(const char* infile_name, const char* outfile_name);

/* CONVERSION PGM TO PPM
 * Converts a pgm image to a ppm image:
 * Parameters: input image, output image
 * Returns: 1 for error, 0 if successful
*/
void convert_pgm_to_ppm(const char* infile_name, const char* outfile_name);

/* DIMENSION OF PPM IMAGE
 * Gets the dimensions of a ppm image
 * Parameters: header, position
 * Returns: dimensions
 */
int getDimension(unsigned char *header, int &pos);

/* WIDTH OF PPM IMAGE
 * Gets the width of a ppm image
 * Parameters: input image
 * Returns: width
 */
int getWidth(const char *filename);

/* HEIGHT OF PPM IMAGE
 * Gets the height of a ppm image
 * Parameters: input image
 * Returns: height
 */
int getHeight(const char *filename);

#endif//PROJECT_UTILS_HPP
