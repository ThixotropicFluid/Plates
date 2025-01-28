#include "perlin.h"
#include <stdlib.h>
#include <math.h>

// Permutation table for Perlin noise
static int p[512];

// Gradient vectors for 3D Perlin noise
static float gradients[12][3] = {
    {1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
    {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
    {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}
};

// Fade function for smooth interpolation
static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation function
static float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

// Dot product of a gradient vector and a distance vector
static float grad(int hash, float x, float y, float z) {
    int h = hash % 12;
    return gradients[h][0] * x + gradients[h][1] * y + gradients[h][2] * z;
}

// Initialize the permutation table with a seed
void setPerlinSeed(unsigned int seed) {
    srand(seed);
    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }
    // Shuffle the permutation table
    for (int i = 0; i < 256; i++) {
        int j = rand() % 256;
        int temp = p[i];
        p[i] = p[j];
        p[j] = temp;
    }
    // Duplicate the permutation table to avoid modulo operations
    for (int i = 0; i < 256; i++) {
        p[256 + i] = p[i];
    }
}

// Perlin noise function
float perlin(float x, float y, float z) {
    // Find the unit cube that contains the point
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;

    // Find relative x, y, z of the point in the cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute fade curves for x, y, z
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // Hash coordinates of the 8 cube corners
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    // Interpolate between the 8 corners
    float res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                       grad(p[BA], x - 1, y, z)),
                               lerp(u, grad(p[AB], x, y - 1, z),
                                       grad(p[BB], x - 1, y - 1, z))),
                       lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                       grad(p[BA + 1], x - 1, y, z - 1)),
                               lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                                       grad(p[BB + 1], x - 1, y - 1, z - 1))));
    return (res + 1.0f) / 2.0f; // Normalize to [0, 1]
}