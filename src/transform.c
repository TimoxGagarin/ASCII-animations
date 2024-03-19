#include "headers/transform.h"

transform_t new_transform(vector3_t position, vector3_t quaternion, vector3_t scale)
{
    return (transform_t){position, quaternion, scale};
}

void transform(vector3_t *p, transform_t *transform)
{
    scale(p, transform->scale);
    rotate(p, transform->quaternion);
    position(p, transform->position);
}