#pragma once

typedef struct vector {
    float x, y;
}vec;

float findDistance(vec vec1, vec vec2);
float findSqrtDistance(vec vec1, vec vec2);
vec findVector(vec vec1, vec vec2);