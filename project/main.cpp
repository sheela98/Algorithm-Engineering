#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <vector>

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

void readPPMHeader(FILE *file, unsigned char *header) {
    fread(header, 15, 1, file);
}

void readPPMImage(FILE *file, unsigned char *image, int size) {
    fread(image, size, 1, file);
}

void writePPM(FILE *file, unsigned char *header, unsigned char *image, int size) {
    fwrite(header, 15, 1, file);
    fwrite(image, size, 1, file);
}

void removeRed(unsigned char *image, unsigned char *withoutredimage, int size) {
    int i;
    for(i = 0; i < size; i+=3) {
        withoutredimage[i] = 0; // set red component to 0
        withoutredimage[i+1] = image[i+1]; // set blue to image
        withoutredimage[i+2] = image[i+1]; // set green to image
    }
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



int main() {
    //ImageReader imageReader();
    FILE *read;
    const char *filename = "/Users/sheela_1/Documents/5_WS_22/Algorithm Engineering/Algorithm-Engineering/project/read_image.ppm";
    read = fopen(filename, "rb"); // open in binary format

    unsigned char header[15], *image;
    readPPMHeader(read, header); // read header
    if (header[0] != 'P' || header[1] != '6') // check header, p6 - binary
    {
        cout << "Wrong file format\n";
        return 0;
    }

    FILE *write;
    // "w" - write (creates an empty file for output operations)
    write = fopen("/Users/sheela_1/Documents/5_WS_22/Algorithm Engineering/Algorithm-Engineering/project/write_image.ppm", "wb");

    int clrs;
    int pos = 3;
    int width = getDimension(header, pos);
    pos++;
    int height = getDimension(header, pos);
    cout << "width: " << width << "\theight: " << height << "\n";

    // pixel: w * h * 3 (bytes - RGB)
    image = new unsigned char [width * height * 3];

    unsigned char *withoutredimage;
    withoutredimage = new unsigned char [width * height * 3];

    readPPMImage(read, image, width*height*3);
    removeRed(image, withoutredimage, width*height*3);
    writePPM(write, header, withoutredimage, width*height*3);

    // Median Filter
    int filterSize = 3;
    medianFilter("input.ppm", "output.ppm", width, height, filterSize);
    //

    fclose(read);
    fclose(write);

    return 0;
}