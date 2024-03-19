#pragma once

typedef struct vector3
{
    float x;
    float y;
    float z;
} vector3_t;

vector3_t new_vector3(float x, float y, float z);
void copy(vector3_t *dest, vector3_t src);
void position(vector3_t *p, vector3_t position);
void rotate(vector3_t *p, vector3_t ax);
void scale(vector3_t *p, vector3_t scale);
float dot(vector3_t vect1, vector3_t vect2);
float length(vector3_t vect);
vector3_t norm(vector3_t vect);