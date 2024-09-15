#include "noise_generator.hpp"

NoiseGenerator::NoiseGenerator(unsigned int seed)
    : m_generator(seed), m_distribution(-1.0f, 1.0f) {}

float NoiseGenerator::noise(float x, float y, float z) const {
  int xi = static_cast<int>(std::floor(x)) & 255;
  int yi = static_cast<int>(std::floor(y)) & 255;
  int zi = static_cast<int>(std::floor(z)) & 255;

  float xf = x - std::floor(x);
  float yf = z - std::floor(y);
  float zf = z - std::floor(z);

  float u = m_fade(xf);
  float v = m_fade(yf);
  float w = m_fade(zf);

  int aaa = m_permutation[m_permutation[m_permutation[xi] + yi] + zi];
  int aba = m_permutation[m_permutation[m_permutation[xi] + yi + 1] + zi];
  int aab = m_permutation[m_permutation[m_permutation[xi] + yi] + zi + 1];
  int abb = m_permutation[m_permutation[m_permutation[xi] + yi + 1] + zi + 1];
  int baa = m_permutation[m_permutation[m_permutation[xi + 1] + yi] + zi];
  int bba = m_permutation[m_permutation[m_permutation[xi + 1] + yi + 1] + zi];
  int bab = m_permutation[m_permutation[m_permutation[xi + 1] + yi] + zi + 1];
  int bbb =
      m_permutation[m_permutation[m_permutation[xi + 1] + yi + 1] + zi + 1];

  // m_interpolate
  float x1 =
      m_interpolate(m_grad(aaa, xf, yf, zf), m_grad(baa, xf - 1, yf, zf), u);
  float x2 = m_interpolate(m_grad(aba, xf, yf - 1, zf),
                           m_grad(bba, xf - 1, yf - 1, zf), u);
  float y1 = m_interpolate(x1, x2, v);

  x1 = m_interpolate(m_grad(aab, xf, yf, zf - 1),
                     m_grad(bab, xf - 1, yf, zf - 1), u);
  x2 = m_interpolate(m_grad(abb, xf, yf - 1, zf - 1),
                     m_grad(bbb, xf - 1, yf - 1, zf - 1), u);
  float y2 = m_interpolate(x1, x2, v);

  return (m_interpolate(y1, y2, w) + 1.0f) / 2.0f;
}

float NoiseGenerator::m_interpolate(float a, float b, float t) const {
  return a + t * (b - a);
}

float NoiseGenerator::m_grad(int hash, float x, float y, float z) const {
  int h = hash & 15;
  float u = h < 8 ? x : y;
  float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float NoiseGenerator::m_fade(float t) const {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

const int NoiseGenerator::m_permutation[512] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
    225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
    6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
    35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
    231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
    76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
    58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
    154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
    145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
    84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
    180,

    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
    225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
    6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
    35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
    231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
    76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
    58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
    154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
    145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
    84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
    180};
