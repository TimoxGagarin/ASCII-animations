#pragma once
#include "vector3.h"

typedef struct transform
{
    vector3_t position;
    vector3_t quaternion;
    vector3_t scale;
} transform_t;

transform_t new_transform(vector3_t position, vector3_t quaternion, vector3_t scale);
void transform(vector3_t *p, transform_t *transform);
