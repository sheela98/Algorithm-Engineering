//
// Created by Sheela on 28/02/23.
//

#include "median.h"
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <vector>
#include <omp.h>

using namespace std;

struct Pixel {
    unsigned char r, g, b;
};

Pixel median(vector<Pixel> v) {
    sort(v.begin(), v.end(), [](Pixel a, Pixel b) {
        return a.r < b.r;
    });
    Pixel p = v[v.size() / 2];
    sort(v.begin(), v.end(), [](Pixel a, Pixel b) {
        return a.g < b.g;
    });
    p.g = v[v.size() / 2].g;
    sort(v.begin(), v.end(), [](Pixel a, Pixel b) {
        return a.b < b.b;
    });
    p.b = v[v.size() / 2].b;
    return p;
}

void medianFilter(string inputFile, string outputFile, int width, int height, int filterSize) {
    FILE* inFile = fopen(inputFile.c_str(), "rb");
    FILE* outFile = fopen(outputFile.c_str(), "wb");
    if (inFile == NULL || outFile == NULL) {
        printf("Error: Cannot open file\n");
        return;
    }

    char magicNumber[3];
    fscanf(inFile, "%s", magicNumber);
    int maxVal;
    fscanf(inFile, "%d", &width);
    fscanf(inFile, "%d", &height);
    fscanf(inFile, "%d", &maxVal);
    fprintf(outFile, "P6\n%d %d\n%d\n", width, height, maxVal);

    int halfSize = filterSize / 2;
    vector<Pixel> window(filterSize * filterSize);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int y = -halfSize; y <= halfSize; ++y) {
                for (int x = -halfSize; x <= halfSize; ++x) {
                    int ii = i + y;
                    int jj = j + x;
                    if (ii < 0) ii = 0;
                    if (ii >= height) ii = height - 1;
                    if (jj < 0) jj = 0;
                    if (jj >= width) jj = width - 1;
                    fseek(inFile, sizeof(char) * (ii * width + jj) * 3, SEEK_SET);
                    fread(&window[(y + halfSize) * filterSize + x + halfSize], sizeof(char), 3, inFile);
                }
            }
            Pixel p = median(window);
            fwrite(&p, sizeof(char), 3, outFile);
        }
    }

    fclose(inFile);
    fclose(outFile);
}

void medianFilter_with_openmp(string inputFile, string outputFile, int width, int height, int filterSize) {
    FILE* inFile = fopen(inputFile.c_str(), "rb");
    FILE* outFile = fopen(outputFile.c_str(), "wb");
    if (inFile == NULL || outFile == NULL) {
        printf("Error: Cannot open file\n");
        return;
    }

    char magicNumber[3];
    fscanf(inFile, "%s", magicNumber);
    int maxVal;
    fscanf(inFile, "%d", &width);
    fscanf(inFile, "%d", &height);
    fscanf(inFile, "%d", &maxVal);
    fprintf(outFile, "P6\n%d %d\n%d\n", width, height, maxVal);

    int halfSize = filterSize / 2;
    vector<Pixel> window(filterSize * filterSize);

    // tried with #pragma omp parallel for and collapse as commented
    // result: slowed down performance from 6 to 10 - 30 seconds

    //#pragma omp parallel for
    for (int i = 0; i < height; ++i) {
        //#pragma omp parallel for
        for (int j = 0; j < width; ++j) {
            //#pragma omp parallel for collapse(2)
            for (int y = -halfSize; y <= halfSize; ++y) {
                //#pragma omp parallel for
                for (int x = -halfSize; x <= halfSize; ++x) {
                    int ii = i + y;
                    int jj = j + x;
                    if (ii < 0) ii = 0;
                    if (ii >= height) ii = height - 1;
                    if (jj < 0) jj = 0;
                    if (jj >= width) jj = width - 1;
                    fseek(inFile, sizeof(char) * (ii * width + jj) * 3, SEEK_SET);
                    fread(&window[(y + halfSize) * filterSize + x + halfSize], sizeof(char), 3, inFile);
                }
            }
            Pixel p = median(window);
            fwrite(&p, sizeof(char), 3, outFile);
        }
    }

    fclose(inFile);
    fclose(outFile);
}



// Convert PPM image to PGM greyscale image
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

int convert_ppm_to_pgm_with_openmp(const char* infile_name, const char* outfile_name) {

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

int getDimension(unsigned char *header, int &pos) {
    int dim = 0;
    // read characters and store as integers
    for(; header[pos]!='\n' && header[pos]!=' '; pos++)
        dim = dim * 10 + (header[pos] - '0');
    return dim;
}

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
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

int main() {
    string input = "images/inputfile.ppm", output = "images/outputfile.ppm";

    const double start = omp_get_wtime(); // measure performance
    int filterSize = 3, mode;
    medianFilter(input, output, getWidth(input.c_str()), getHeight(input.c_str()), filterSize);
    cout << "Median normal: "<< omp_get_wtime() - start << " seconds" << endl; // measure performance

    const double start_2 = omp_get_wtime(); // measure performance
    medianFilter_with_openmp(input, output, getWidth(input.c_str()), getHeight(input.c_str()), filterSize);
    cout << "Median with omp: "<< omp_get_wtime() - start_2 << " seconds" << endl; // measure performance


    const double start_3 = omp_get_wtime(); // measure performance
    convert_ppm_to_pgm(input.c_str(), output.c_str());
    cout << "Convert ppm to pgm normal: "<< omp_get_wtime() - start_3 << " seconds" << endl; // measure performance

    const double start_4 = omp_get_wtime(); // measure performance
    convert_ppm_to_pgm_with_openmp(input.c_str(), output.c_str());
    cout << "Convert ppm to pgm with omp: "<< omp_get_wtime() - start_4 << " seconds" << endl; // measure performance

}