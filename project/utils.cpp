//
// Created by Sheela on 28/02/23.
//

#include "utils.hpp"
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <omp.h>

using namespace std;

void image_enhancer(const char *input, const char *output) {
    apply_median_filter(input, output);
    apply_adaptive_thresholding(output, output);
}

Pixel* read_ppm(const char* filename, int& width, int& height) {
    // Open the file
    FILE* file = fopen(filename, "rb");
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        exit(1);
    }

    // Read the magic number
    char magic[2];
    fread(magic, 1, 2, file);
    if (magic[0] != 'P' || magic[1] != '6') {
        cerr << "Error: file " << filename << " is not a ppm image file" << endl;
        exit(1);
    }

    // Read the image dimensions
    fscanf(file, "%d %d\n", &width, &height);

    // Read the maximum color value
    int maxColor;
    fscanf(file, "%d\n", &maxColor);
    if (maxColor != 255) {
        cerr << "Error: file " << filename << " has invalid maximum color value" << endl;
        exit(1);
    }

    // Read the pixel data
    Pixel* image = new Pixel[width*height];
    fread(image, sizeof(Pixel), width*height, file);

    // Close the file
    fclose(file);

    return image;
}

void write_ppm(const char* filename, Pixel* image, int width, int height) {
    // Open the file
    FILE* file = fopen(filename, "wb");
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        exit(1);
    }

    // Write the magic number and image dimensions
    fprintf(file, "P6\n%d %d\n%d\n", width, height, 255);

    // Write the pixel data
    fwrite(image, sizeof(Pixel), width*height, file);

    // Close the file
    fclose(file);
}

void median_filter(Pixel* image, int width, int height) {
    // Create a temporary image buffer
    Pixel* temp = new Pixel[width*height];

#pragma omp parallel for collapse(2)
    // Loop through all pixels in the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Create a list of pixel values in the 2x2 window around the current pixel
            Pixel window[4];
            int index = 0;
            for (int dy = -1; dy <= 0; dy++) {
                for (int dx = -1; dx <= 0; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        window[index++] = image[ny*width + nx];
                    }
                }
            }

            // Sort the pixel values by intensity
            sort(window, window+index, [](Pixel a, Pixel b) {
                return 0.299*a.r + 0.587*a.g + 0.114*a.b < 0.299*b.r + 0.587*b.g + 0.114*b.b;
            });

            // Set the current pixel value to the median value in the window
            temp[y*width + x] = window[index/2];
        }
    }

    // Copy the temporary image buffer back to the original image buffer
    memcpy(image, temp, width*height*sizeof(Pixel));

    // Free the temporary image buffer
    delete[] temp;
}

void apply_median_filter(const char *input, const char *output) {
    int width, height;
    Pixel* image = read_ppm(input, width, height);
    median_filter(image, width, height);
    write_ppm(output, image, width, height);

    delete[] image;
}

int convert_ppm_to_pgm(const char* infile_name, const char* outfile_name) {
    // open the input file
    FILE* infile = fopen(infile_name, "rb");
    if (!infile) {
        fprintf(stderr, "Error: could not open input file %s\n", infile_name);
        return 1;
    }

    // read the magic number
    char magic_number[3];
    if (fscanf(infile, "%2s", magic_number) != 1 || magic_number[0] != 'P' || magic_number[1] != '6') {
        fprintf(stderr, "Error: input file is not in PPM format\n");
        fclose(infile);
        return 1;
    }

    // read the image width, height, and max color value
    int width, height, maxval;
    if (fscanf(infile, "%d %d %d\n", &width, &height, &maxval) != 3 || maxval > 255) {
        fprintf(stderr, "Error: invalid PPM format\n");
        fclose(infile);
        return 1;
    }

    // allocate memory for the RGB pixel data
    vector<unsigned char> data(width * height * 3);

    // read the pixel data
    if (fread(data.data(), 1, data.size(), infile) != data.size()) {
        fprintf(stderr, "Error: could not read pixel data\n");
        fclose(infile);
        return 1;
    }

    // close the input file
    fclose(infile);

    // open the output file
    FILE* outfile = fopen(outfile_name, "wb");
    if (!outfile) {
        fprintf(stderr, "Error: could not open output file\n");
        return 1;
    }

    // write the PGM header
    fprintf(outfile, "P5\n%d %d\n255\n", width, height);

    // allocate memory for the grayscale pixel data
    vector<unsigned char> graydata(width * height);

    // convert the RGB pixel data to grayscale
#pragma omp parallel for
    for (int i = 0; i < width * height; i++) {
        unsigned char r = data[i * 3];
        unsigned char g = data[i * 3 + 1];
        unsigned char b = data[i * 3 + 2];
        graydata[i] = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
    }

    // write the grayscale pixel data to the output file
    if (fwrite(graydata.data(), 1, graydata.size(), outfile) != graydata.size()) {
        fprintf(stderr, "Error: could not write pixel data\n");
        fclose(outfile);
        return 1;
    }

    // close the output file
    fclose(outfile);

    return 0;
}

void convert_pgm_to_ppm(const char* input_file_path, const char* output_file_path) {
    // Open the input file in read mode
    FILE* input_file = fopen(input_file_path, "rb");
    if (!input_file) {
        printf("Failed to open input file %s\n", input_file_path);
        return;
    }

    // Read the header information from the input file
    char header[3];
    int width, height, max_value;
    fscanf(input_file, "%s\n%d %d\n%d\n", header, &width, &height, &max_value);

    // Create a new header for the output file
    char new_header[256];
    sprintf(new_header, "P6\n%d %d\n%d\n", width, height, max_value);

    // Open the output file in write binary mode and write the new header
    FILE* output_file = fopen(output_file_path, "wb");
    if (!output_file) {
        printf("Failed to open output file %s\n", output_file_path);
        fclose(input_file);
        return;
    }
    fprintf(output_file, "%s", new_header);

    // Read the pixel data from the input file and convert it to RGB values
    unsigned char* pixel_data = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    fread(pixel_data, sizeof(unsigned char), width * height, input_file);
    unsigned char* rgb_data = (unsigned char*)malloc(3 * width * height * sizeof(unsigned char));
#pragma omp parallel for
    for (int i = 0; i < width * height; i++) {
        rgb_data[3 * i] = pixel_data[i];
        rgb_data[3 * i + 1] = pixel_data[i];
        rgb_data[3 * i + 2] = pixel_data[i];
    }

    // Write the RGB pixel data to the output file
    fwrite(rgb_data, sizeof(unsigned char), 3 * width * height, output_file);

    // Close the input and output files and free the allocated memory
    fclose(input_file);
    fclose(output_file);
    free(pixel_data);
    free(rgb_data);
}

void adaptive_thresholding(const char *input_image, const char *output_image, int blockSize, int threshold_offset) {
    // Load the PGM image
    FILE* inputFile = fopen(input_image, "rb");
    if (!inputFile) {
        cerr << "Error: Could not open input file." << endl;
        return;
    }
    char format[3];
    int width, height, maxVal;
    fscanf(inputFile, "%2s %d %d %d", format, &width, &height, &maxVal);
    if (strcmp(format, "P5") != 0) {
        cerr << "Error: Invalid PGM format." << endl;
        return;
    }
    vector<unsigned char> imageData(width * height);
    fread(imageData.data(), sizeof(unsigned char), width * height, inputFile);
    fclose(inputFile);

    // Apply adaptive thresholding
    vector<unsigned char> thresholdedData(width * height);

#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;
            int count = 0;
#pragma omp parallel for collapse(2)
            for (int j = y - blockSize / 2; j <= y + blockSize / 2; ++j) {
                for (int i = x - blockSize / 2; i <= x + blockSize / 2; ++i) {
                    if (i >= 0 && i < width && j >= 0 && j < height) {
                        sum += imageData[j * width + i];
                        ++count;
                    }
                }
            }
            int mean = sum / count;
            int threshold = mean - threshold_offset;
            thresholdedData[y * width + x] = imageData[y * width + x] > threshold ? 255 : 0;
        }
    }

    // Save the thresholded image
    FILE* outputFile = fopen(output_image, "wb");
    if (!outputFile) {
        cerr << "Error: Could not open output file." << endl;
        return;
    }
    fprintf(outputFile, "P5\n%d %d\n%d\n", width, height, maxVal);
    fwrite(thresholdedData.data(), sizeof(unsigned char), width * height, outputFile);
    fclose(outputFile);

}

int getDimension(unsigned char *header, int &pos) {
    int dim = 0;
    // read characters and store as integers
    for(; header[pos]!='\n' && header[pos]!=' '; pos++)
        dim = dim * 10 + (header[pos] - '0');
    return dim;
}

void apply_adaptive_thresholding(const char *input, const char *output) {
    const char *output_pgm = "images/img.pgm";
    // Convert ppm to pgm image
    convert_ppm_to_pgm(input, output_pgm);

    // Apply adaptive thresholding to converted image and write to output_pgm
    adaptive_thresholding(output_pgm, output_pgm, 31, 15);

    // Convert output_pgm to output ppm image
    convert_pgm_to_ppm(output_pgm, output);
}

int getWidth(const char *filename) {
    FILE *read, *write;
    read = fopen(filename, "rb"); // open in binary format

    unsigned char header[15], *image;
    fread(header, 15, 1, read);
    if (header[0] != 'P' || header[1] != '6') // check header, p6 - binary
    {
        fprintf(stderr, "Wrong file format\n");
    }

    int pos = 3;
    int width = getDimension(header, pos);
    return width;
}

int getHeight(const char *filename) {
    FILE *read, *write;
    read = fopen(filename, "rb"); // open in binary format

    unsigned char header[15], *image;
    fread(header, 15, 1, read);
    if (header[0] != 'P' || header[1] != '6') // check header, p6 - binary
    {
        fprintf(stderr, "Wrong file format\n");
    }

    int pos = 4;
    int height = getDimension(header, pos);
    return height;
}