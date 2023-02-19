#include <iostream>
#include <vector>

int main() {
  std::vector<int> v(1000, 7);
  int64_t sum = 0;
  for (int i = 0; i < v.size(); ++i) {
    sum += v[i];
  }
  std::cout << sum << std::endl;
}
