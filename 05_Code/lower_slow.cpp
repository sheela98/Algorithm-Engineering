#include <algorithm>
#include <cstring>
#include <iostream>
#include <omp.h>

using namespace std;

// fill char array of size n with random alphanumeric strings
void gen_random(char *s, const int64_t n) {
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
  for (int64_t i = 0; i < n; ++i) {
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }
  s[n] = 0;
}

// generates a random alphanumeric string of size n
string generate_alpha_numeric_string(const int64_t n) {
  string chars{};
  chars.resize(n);
  char *data_ptr = const_cast<char *>(chars.data());
  gen_random(data_ptr, n);
  return chars;
}

// convert char array to lower case
void lower(char *s) {
  // 'a' is greater than 'A' in ASCII
  constexpr char addend = 'a' - 'A';
  for (size_t i = 0; i < strlen(s); i++) {
    if ('A' <= s[i] && s[i] <= 'Z') {
      s[i] += addend;
    }
  }
}

int main() {
  // create 2 identical random strings of size n
  int64_t n = 100000;
  string chars = generate_alpha_numeric_string(n);
  char *data_ptr = const_cast<char *>(chars.data());
  string chars2 = chars; // copy chars into chars2

  // custom implementation of converting a string to lower case
  double start = omp_get_wtime();
  lower(data_ptr);
  cout << "custom: " << omp_get_wtime() - start << " seconds" << endl;

  // the C++ way of converting a string to lower case
  start = omp_get_wtime();
  std::transform(chars2.begin(), chars2.end(), chars2.begin(),
            [](char c) { return std::tolower(c); });
  cout << "C++: " << omp_get_wtime() - start << " seconds" << endl;

  // test if strings are the same
  cout << boolalpha << "same: " << ((chars == chars2)) << endl;
}
