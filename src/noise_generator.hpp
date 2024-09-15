#pragma once

#include <random>

class NoiseGenerator {
private:
  std::mt19937 m_generator;
  std::uniform_real_distribution<float> m_distribution;

  static const int m_permutation[512];

  float m_fade(float t) const;
  float m_interpolate(float a, float b, float t) const;
  float m_grad(int hash, float x, float y, float z) const;

public:
  NoiseGenerator(unsigned int seed);

  float noise(float x, float y, float z) const;
};
