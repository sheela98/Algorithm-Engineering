#include <immintrin.h>
#include <iostream>

/********** scalar Xoroshiro128+ **********/
inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k)); // rotate
}

inline uint64_t next(uint64_t &s0, uint64_t &s1) {
  s1 ^= s0;                            // modify s1
  s0 = rotl(s0, 24) ^ s1 ^ (s1 << 16); // modify s0
  s1 = rotl(s1, 37);                   // modify s1
  return s0 + s1;
}

/********** vectorized Xoroshiro128+ **********/
#define VROTL(x, k)                                          \
  _mm256_or_si256(_mm256_slli_epi64((x), (k)),               \
                  _mm256_srli_epi64((x), 64 - (k))) // rotate

inline __m256i vnext(__m256i &s0, __m256i &s1) {
  s1 = _mm256_xor_si256(s0, s1);                    // modify s1
  s0 = _mm256_xor_si256(_mm256_xor_si256(VROTL(s0, 24), s1),
                        _mm256_slli_epi64(s1, 16)); // modify s0
  s1 = VROTL(s1, 37);                               // modify s1
  return _mm256_add_epi64(s0, s1);
}

int main() {
  {
    uint64_t s0 = 1314472907419283471; // seed 0
    uint64_t s1 = 7870872464127966567; // seed 1
    for (int i = 0; i < 4; ++i) {
      std::cout << next(s0, s1) << std::endl;
    }
  }
  std::cout << std::endl;
  {
    auto s0 = _mm256_setr_epi64x(8265987198341093849, 3762817312854612374,
                                 1324281658759788278, 6214952190349879213); // vector seed 0
    auto s1 = _mm256_setr_epi64x(2874178529384792648, 1257248936691237653,
                                 7874578921548791257, 1998265912745817298); // vector seed 1
    auto random_vec = vnext(s0, s1);
    for (int i = 0; i < 4; ++i) {
      std::cout << ((uint64_t *)&random_vec)[i] << std::endl;
    }
    // generate next random vector with vnext(s0, s1)
  }
}
