#include "utils.h"
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

int main() {

    string input, output;

    cout << "Enhancer for Scanned Images - Console Application" << endl;
    cout << "Input format: images/img_00.ppm" << endl;

    //cin >> input;
    bool exit_requested = false; // Flag to indicate if the user has requested to exit

    while (!exit_requested) {
        cout << "Enter the path for the image to enhance (or 'exit' to quit): ";
        cin >> input;

        if (strcmp(input.c_str(), "exit") == 0) {
            exit_requested = true; // User has requested to exit
        } else {
            cout << "Enter the path for the output image (or 'exit to quit): ";
            cin >> output;

            if (strcmp(output.c_str(), "exit") == 0) {
                exit_requested = true; // User has requested to exit
            } else {
                image_enhancer(input.c_str(), output.c_str());// Enhance the image
                cout << "The image is stored to " << output << endl;
            }
        }
    }



    return 0;

}