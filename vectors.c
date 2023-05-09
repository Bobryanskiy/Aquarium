#include "vectors.h"
#include <math.h>

float findDistance(vec vec1, vec vec2) {
    vec t = findVector(vec1, vec2);
    return t.x * t.x + t.y * t.y;
}

float findSqrtDistance(vec vec1, vec vec2) {
    return sqrt(findDistance(vec1, vec2));
}

vec findVector(vec vec1, vec vec2) {
    vec vec;
    vec.x = vec2.x - vec1.x;
    vec.y = vec2.y - vec1.y;
    return vec;
}
