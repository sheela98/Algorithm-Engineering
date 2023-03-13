//
// Created by Sheela on 28/02/23.
//


#ifndef PROJECT_UTILS_HPP
#define PROJECT_UTILS_HPP

#include <string>
using namespace std;

/* MEDIAN FILTER
 * Computes the median filter of an input image and writes the processed image to an output image.
 * Parameters: input image, output image, width, height, filter size
 * Returns: void
 */
void medianFilter(string inputFile, string outputFile, int width, int height, int filterSize);


/* ADAPTIVE THRESHOLDING
 * Does image processing on a given input image with adaptive thresholding
 * the processed image is written to a different file.
 * Parameters: input image, output image, block size, threshold offset
 * Returns: void
 */
void adaptive_thresholding(const char *input_image, const char *output_image, int blockSize, int threshold_offset);

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
