cmake_minimum_required(VERSION 3.23)
set(CMAKE_OSX_ARCHITECTURES x86_64)
project(project)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS “{CMAKE_CXX_FLAGS} -0fast -std=c++11 -fopenmp“)
# set(CXXFLAGS += -I/usr/local/Cellar/catch2/3.3.2/include -L/usr/local/Cellar/catch2/3.3.2/lib)

# Set up Catch2
find_package(Catch2)

# search for OpenMP, REQUIRES -> fatal error if not found
find_package(OpenMP REQUIRED)

# test.cpp test.h
add_executable(main main.cpp utils.cpp utils.h test.cpp test.h)

# Copy the images directory to the build directory
file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# link to the imported target provided by the FindOpenMP module
target_link_libraries(main OpenMP::OpenMP_CXX)
# link to the imported target provided by the Catch2WithMain module
target_link_libraries(main Catch2::Catch2WithMain)

