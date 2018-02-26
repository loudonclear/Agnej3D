#include "PerlinNoise.h"

#include <numeric>
#include <cmath>
#include <algorithm>
#include <random>

PerlinNoise::PerlinNoise(unsigned int seed)
{
    setSeed(seed);
}

void PerlinNoise::setSeed(unsigned int seed) {
    std::default_random_engine rnd(seed);

    p.resize(256);
    std::iota(p.begin(), p.end(), 0);

    std::shuffle(p.begin(), p.end(), rnd);
    p.insert(p.end(), p.begin(), p.end());
}

float PerlinNoise::noise(float x, float y, float z) {

    int fx = (int)floor(x), fy = (int)floor(y), fz = (int)floor(z);
    int cx = fx & 255, cy = fy & 255, cz = fz & 255;

    x -= fx; y -= fy; z -= fz;

    float u = fade(x), v = fade(y), w = fade(z);

    int A = p[cx] + cy, AA = p[A] + cz, AB = p[A+1] + cz;
    int B = p[cx+1] + cy, BA = p[B] + cz, BB = p[B+1] + cz;

    return lerp(w,
                lerp(v,
                     lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)),
                     lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),
                lerp(v,
                     lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)),
                     lerp(u, grad(p[AB+1], x, y-1, z-1), grad(p[BB+1], x-1, y-1, z-1))));
}

float PerlinNoise::fade(float t) {
    return ((t * 6 - 15) * t + 10) * t * t * t;
}

float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h==12 || h==14 ? x : z;

    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}
