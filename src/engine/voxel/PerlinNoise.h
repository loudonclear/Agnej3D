#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>

class PerlinNoise
{
public:
    PerlinNoise(unsigned int seed);

    void setSeed(unsigned int seed);

    float noise(float x, float y, float z);

private:
    inline float fade(float t);
    inline float lerp(float t, float a, float b);
    inline float grad(int hash, float x, float y, float z);


    std::vector<int> p;
};

#endif // PERLINNOISE_H
