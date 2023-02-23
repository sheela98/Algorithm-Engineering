//
// Created by Sheela on 23/02/23.
//

#include "adaptive_thresholding.h"

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;


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
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;
            int count = 0;
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

int main()
{
    const char *input_image = "images/image.pgm";
    const char *output_image = "images/output.pgm";
    adaptive_thresholding(input_image, output_image, 31, 15);

    return 0;
}
