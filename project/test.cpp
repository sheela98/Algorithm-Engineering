//
// Created by Sheela on 13/03/23.
//

#include "test.h"
#define CONFIG_CATCH_MAIN
#include "catch2/catch_all.hpp"
#include "utils.h"
#include <filesystem>

// READ PPM
TEST_CASE("Test read_ppm function", "[read_ppm]") {
    int width, height;
    REQUIRE_THROWS_WITH(read_ppm("images/invalid.ppm", width, height), "Error: could not open file images/invalid.ppm");
}

// WRITE PPM
TEST_CASE("Test write_ppm function", "[write_ppm]") {
    const char *output = "images/test_write.ppm";

    int width, height;
    Pixel *image;
    write_ppm(output,image, width, height);

    // Check if path exists
    bool exists = std::__fs::filesystem::exists(output);
    REQUIRE(exists == true);
}

// CONVERT PPM TO PGM
TEST_CASE("Test convert_ppm_to_pgm", "[convert_ppm_to_pgm]") {
    const char *input = "images/test.ppm";
    const char *output = "images/test_to_pgm.pgm";
    convert_ppm_to_pgm(input, output);

    // Check if path exists
    bool exists = std::__fs::filesystem::exists(output);
    REQUIRE(exists == true);

    REQUIRE_THROWS_WITH(convert_ppm_to_pgm("images/invalid.ppm", output), "Error: could not open input file images/invalid.ppm");
}

// CONVERT PGM TO PPM
TEST_CASE("Test convert_pgm_to_ppm", "[convert_pgm_to_ppm]") {
    const char *input = "images/test.pgm";
    const char *output = "images/test_to_ppm.ppm";
    convert_pgm_to_ppm(input, output);

    // Check if path exists
    bool exists = std::__fs::filesystem::exists(output);
    REQUIRE(exists == true);

    REQUIRE_THROWS_WITH(convert_pgm_to_ppm("images/invalid.pgm", output), "Failed to open input file images/invalid.pgm");
}

// GET HEIGHT
TEST_CASE("Test getHeight", "[getHeight]") {
    REQUIRE(getHeight("images/img_00.ppm") == 200);
    REQUIRE(getHeight("images/img_05.ppm") == 94);
}

// GET WIDTH
TEST_CASE("Test getWidth", "[getWidth]") {
    REQUIRE(getWidth("images/img_00.ppm") == 1200);
    REQUIRE(getWidth("images/img_05.ppm") == 194);
}

