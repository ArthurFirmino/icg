#pragma once

#include <cstdlib>
#include "OpenGP/GL/Application.h"

using namespace OpenGP;

inline float lerp(float x, float y, float t) {
    /// TODO: Implement linear interpolation between x and y
    return 0.0f;
}

inline float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float rand01() {
    return ((float) std::rand())/((float) RAND_MAX);
}

float* perlin2D(const int width, const int height, const int period=64);

/// Generates a heightmap using fractional brownian motion
R32FTexture* fBm2DTexture() {

    ///--- Precompute perlin noise on a 2D grid
    const int width = 512;
    const int height = 512;
    float *perlin_data = perlin2D(width, height, 128);

    ///--- fBm parameters
    float H = 0.8f;
    float lacunarity = 2.0f;
    float offset = 0.1f;
    const int octaves = 4;

    ///--- Initialize to 0s
    float *noise_data = new float[width*height];
    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {
            noise_data[i+j*height] = 0;
        }
    }

    ///--- Precompute exponent array
    float *exponent_array = new float[octaves];
    float f = 1.0f;
    for (int i = 0; i < octaves; ++i) {
        /// TODO: Implement this step according to Musgraves paper on fBM

    }

    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {
            int I = i;
            int J = j;
            for(int k = 0; k < octaves; ++k) {
                /// TODO: Get perlin noise at I,J, add offset, multiply by proper term and add to noise_data
                noise_data[i+j*height] += 0;

                ///--- Point to sample at next octave
                I *= (int) lacunarity;
                J *= (int) lacunarity;
            }
        }
    }

    R32FTexture* _tex = new R32FTexture();
    _tex->upload_raw(width, height, noise_data);

    delete perlin_data;
    delete noise_data;
    delete exponent_array;

    return _tex;
}

float* perlin2D(const int width, const int height, const int period) {

    ///--- Precompute random gradients
    float *gradients = new float[width*height*2];
    auto sample_gradient = [&](int i, int j) {
        float x = gradients[2*(i+j*height)];
        float y = gradients[2*(i+j*height)+1];
        return Vec2(x,y);
    };

    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {
            float angle = rand01();
            gradients[2*(i+j*height)] = cos(2 * angle * M_PI);
            gradients[2*(i+j*height)+1] = sin(2 * angle * M_PI);
        }
    }

    ///--- Perlin Noise parameters
    float frequency = 1.0f / period;

    float *perlin_data = new float[width*height];
    for (int i = 0; i < width; ++ i) {
        for (int j = 0; j < height; ++ j) {

            ///--- Integer coordinates of corners
            int left = (i / period) * period;
            int right = (left + period) % width;
            int top = (j / period) * period;
            int bottom = (top + period) % height;

            ///--- local coordinates [0,1] within each block
            float dx = (i - left) * frequency;
            float dy = (j - top) * frequency;

            ///--- Fetch random vectors at corners
            Vec2 topleft = sample_gradient(left, top);
            Vec2 topright = sample_gradient(right, top);
            Vec2 bottomleft = sample_gradient(left, bottom);
            Vec2 bottomright = sample_gradient(right, bottom);

            ///--- Vector from each corner to pixel center
            Vec2 a(dx,      -dy); // topleft
            Vec2 b(dx-1,    -dy); // topright
            Vec2 c(dx,      1 - dy); // bottomleft
            Vec2 d(dx-1,    1 - dy); // bottomright

            ///TODO: Get scalars at corners HINT: take dot product of gradient and corresponding direction
            float s = 0;
            float t = 0;
            float u = 0;
            float v = 0;

            ///TODO: Interpolate along "x" HINT: use fade(dx) as t
            float st = 0;
            float uv = 0;

            ///TODO: Interpolate along "y"
            float noise = 0;

            perlin_data[i+j*height] = noise;
        }
    }

    delete gradients;
    return perlin_data;
}
