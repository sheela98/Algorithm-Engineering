#include "aligned_allocator.h"
#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

template <class T>
using aligned_vector = std::vector<T, alligned_allocator<T, 64>>;

// point class for constructing arrays of struct
template <class T> struct Point {
  T x;
  T y;
  T z;
};

// structure of arrays
template <class T> class Points {
public:
  T *x;
  T *y;
  T *z;

  size_t size() noexcept { return _size; }

  void set_point(const T &x_coord, const T &y_coord, const T &z_coord,
                 const size_t index) {
    x[index] = x_coord;
    y[index] = y_coord;
    z[index] = z_coord;
  }

  explicit Points(size_t size) : _size(size) {
    _x.reserve(size);
    _y.reserve(size);
    _z.reserve(size);
    x = _x.data();
    y = _y.data();
    z = _z.data();
  }

private:
  aligned_vector<T> _x;
  aligned_vector<T> _y;
  aligned_vector<T> _z;
  size_t _size;
};

int main(int argc, char **argv) {
  int size = 512;

  auto a = aligned_vector<Point<float>>(size);
  auto b = aligned_vector<Point<float>>(size);
  auto squared_distances = aligned_vector<float>(size);

  auto a_soa = Points<float>(size);
  auto b_soa = Points<float>(size);
  auto squared_distances_soa = aligned_vector<float>(size);

  // initialize points
  for (int i = 0; i < size; ++i) {
    auto _i = float(i);
    a[i] = Point<float>{_i + 1.0f, _i + 2.0f, _i + 3.0f};
    b[i] = Point<float>{_i + 2.0f, _i + 3.0f, _i + 4.0f};
    a_soa.set_point(_i + 1.0f, _i + 2.0f, _i + 3.0f, i);
    b_soa.set_point(_i + 2.0f, _i + 3.0f, _i + 4.0f, i);
  }

  auto start = omp_get_wtime();
  // compute the square of the distance between pairs of points (array of structs)
  for (int j = 0; j < 10000000; ++j) {
    for (int i = 0; i < size; ++i) {
      float dx = a[i].x - b[i].x;
      float dy = a[i].y - b[i].y;
      float dz = a[i].z - b[i].z;
      squared_distances[i] = dx * dx + dy * dy + dz * dz;
    }
  }
  cout << "array of structs: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  // compute the square of the distance between pairs of points (structure of arrays)
  for (int j = 0; j < 10000000; ++j) {
    for (int i = 0; i < size; ++i) {
      float dx = a_soa.x[i] - b_soa.x[i];
      float dy = a_soa.y[i] - b_soa.y[i];
      float dz = a_soa.z[i] - b_soa.z[i];
      squared_distances_soa[i] = dx * dx + dy * dy + dz * dz;
    }
  }
  cout << "structure of arrays: " << omp_get_wtime() - start << " seconds"
       << endl;

  if(squared_distances != squared_distances_soa){
    cout << "failed!!!";
  }
}