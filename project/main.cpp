#include <iostream>
#include <omp.h>
#include <algorithm>
#include <cstring>
#include "utils.hpp"

using namespace std;





int main() {

    /*
    const char *input_ppm = "images/img_09.ppm";
    const char *output_ppm = "images/output.ppm";

    const double start = omp_get_wtime(); // measure performance
    image_enhancer(input_ppm, output_ppm);
    cout << omp_get_wtime() - start << " seconds" << endl; // measure performance
    */


    string input, output = "images/output.ppm";

    cout << "Enhancer for Scanned Images - Console Application" << endl;
    cout << "Enter the name of the input image. The images are stored to images/output.ppm." << endl;
    cout << "Input format: images/file_name.ppm" << endl;

    cin >> input;
    image_enhancer(input.c_str(), output.c_str());



    return 0;

}