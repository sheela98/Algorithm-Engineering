//
// Created by Sheela on 20/02/23.
//

#include "otsu.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>

using namespace std;

vector<vector<int>> read_pgm(const char *filename) {
    FILE *infile = fopen(filename, "rb");
    if(!infile) {
        fprintf(stderr, "No input file");
        exit(1);
    }

    char magic[3], comment[100];
    int width, height, maxval;
    fscanf(infile, "%2s\n%100[^\n]\n%d %d\n%d\n", magic, comment, &width, &height, &maxval);
    vector<vector<int>> image(height, vector<int>(width));
    fread(image[0].data(), 1, height*width, infile);
    fclose(infile);
    return image;
}

void write_pgm(const vector<vector<int>>& image, const char* filename) {
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    const int height = image.size();
    const int width = image[0].size();
    fprintf(outfile, "P5\n%d %d\n255\n", width, height);
    fwrite(image[0].data(), 1, height*width, outfile);
    fclose(outfile);
}

// Function to compute the Otsu threshold
int otsu_threshold(const vector<vector<int>>& image) {
    const int height = image.size();
    const int width = image[0].size();
    vector<int> hist(256, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            hist[image[i][j]]++;
        }
    }
    const int total = height * width;
    double sum = 0.0;
    for (int i = 0; i < 256; i++) {
        sum += i * hist[i];
    }
    double sumB = 0.0;
    int wB = 0;
    int wF = 0;
    double varMax = 0.0;
    int threshold = 0;
    for (int i = 0; i < 256; i++) {
        wB += hist[i];
        if (wB == 0) {
            continue;
        }
        wF = total - wB;
        if (wF == 0) {
            break;
        }
        sumB += i * hist[i];
        const double mB = sumB / wB;
        const double mF = (sum - sumB) / wF;
        const double varBetween = wB * wF * pow(mB - mF, 2);
        if (varBetween > varMax) {
            varMax = varBetween;
            threshold = i;
        }
    }
    return threshold;
}

// Function to binarize a pgm image using Otsu thresholding
vector<vector<int>> binarize_otsu(const vector<vector<int>>& image) {
    const int threshold = otsu_threshold(image);
    const int height = image.size();
    const int width = image[0].size();
    vector<vector<int>> binary_image(height, vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            binary_image[i][j] = (image[i][j] >= threshold) ? 255 : 0;
        }
    }
    return binary_image;
}

int main() {
    const char *input_file = "input.pgm", *output_file = "output.pgm";
    vector<vector<int>> input_image, output_image;
    input_image = read_pgm(input_file);
    output_image = binarize_otsu(input_image);
    write_pgm(output_image, output_file);
}