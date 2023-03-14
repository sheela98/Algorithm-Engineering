#include <iostream>
#include <omp.h>
#include <algorithm>
#include <cstring>
#include "utils.hpp"

using namespace std;





int main() {
    const char *input_ppm = "images/img_05.ppm";
    const char *output_ppm = "images/output.ppm";

    //const double start = omp_get_wtime(); // measure performance
    //apply_median_filter(input_ppm, output_ppm);
    //cout << omp_get_wtime() - start << " seconds" << endl; // measure performance

    apply_adaptive_thresholding(input_ppm, output_ppm);

    /*

    const char *input_pgm = "images/image.pgm";
    const char *input_ppm = "images/image.ppm";
    const char *output_pgm = "images/output.pgm";
    const char *output_ppm = "images/output.ppm";

    const double start = omp_get_wtime(); // measure performance
    convert_pgm_to_ppm(input_pgm, output_ppm);
    cout << omp_get_wtime() - start << " seconds" << endl; // measure performance

    const double start_2 = omp_get_wtime(); // measure performance
    convert_ppm_to_pgm(input_ppm, output_pgm);
    cout << omp_get_wtime() - start << " seconds" << endl; // measure performance

    const double start_3 = omp_get_wtime(); // measure performance
    const char *input_image = "images/image.pgm";
    const char *output_image = "images/output.pgm";
    adaptive_thresholding(input_image, output_image, 31, 15);
    cout << omp_get_wtime() - start_3 << " seconds" << endl; // measure performance

    const char *input = "images/output.ppm";
    const char *output = "images/output_07.ppm";
    const char *adaptive = "images/img_07_output.pgm";

    medianFilter(input, output, getWidth(input), getHeight(input), 3);
    convert_ppm_to_pgm(input, output_pgm);
    adaptive_thresholding(output_pgm, adaptive, 31, 15);


    /*
    string input, output;
    int filterSize = 3, mode;

    cout << "Enhancer for Scanned Images - Console Application" << endl;
    cout << "Enter the name of the input image. Format: file.ppm" << endl;

    while(input != "exit") {
        cin >> input;

        cout << "Enter the mode. 1 for Median Filter, 2 for Conversion to PPM to PGM";
        cin >> mode;

        switch (mode) {
            case 1:
                cout << "Mode 1";
                // Median Filter
                medianFilter(input, output, getWidth(input.c_str()), getHeight(input.c_str()), filterSize);
                break;
            case 2:
                if (convert_ppm_to_pgm(input.c_str(), output.c_str()) != 0) {
                    fprintf(stderr, "Error: conversion failed\n");
                    return 1;
                }
                break;
        }
    }
     */
    return 0;

}