#include <math.h>
#include "headers/vector3.h"

vector3_t new_vector3(float x, float y, float z)
{
    return (vector3_t){x, y, z};
}

void copy(vector3_t *dest, vector3_t src)
{
    dest->x = src.x;
    dest->y = src.y;
    dest->z = src.z;
}

void position(vector3_t *p, vector3_t position)
{
    p->x += position.x;
    p->y += position.y;
    p->z += position.z;
}

void rotate(vector3_t *p, vector3_t ax)
{
    vector3_t result;
    float cos_x = cos(ax.x); // 1
    float cos_y = cos(ax.y); // 1
    float cos_z = cos(ax.z); // 0
    float sin_x = sin(ax.x); // 0
    float sin_y = sin(ax.y); // 0
    float sin_z = sin(ax.z); // 1
    result.x = cos_y * cos_z * p->x - sin_z * cos_y * p->y + sin_y * p->z;
    result.y = (sin_x * sin_y * cos_z + sin_z * cos_x) * p->x + (-sin_x * sin_y * sin_z + cos_x * cos_z) * p->y - sin_x * cos_y * p->z;
    result.z = (sin_x * sin_z - sin_y * cos_x * cos_z) * p->x + (sin_x * cos_z + sin_y * sin_z * cos_x) * p->y + cos_x * cos_y * p->z;
    copy(p, result);
}

void scale(vector3_t *p, vector3_t scale)
{
    p->x *= scale.x;
    p->y *= scale.y;
    p->z *= scale.z;
}

float dot(vector3_t vect1, vector3_t vect2)
{
    return vect1.x * vect2.x + vect1.y * vect2.y + vect1.z * vect2.z;
}

float length(vector3_t vect)
{
    return sqrt(dot(vect, vect));
}

vector3_t norm(vector3_t vect)
{
    float len = length(vect);
    return new_vector3(vect.x / len, vect.y / len, vect.z / len);
}