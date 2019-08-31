#ifndef OVERTHEWEEKEND2_HITABLE_H
#define OVERTHEWEEKEND2_HITABLE_H

#include "ray.h"

class material;

struct hit_record{
    float t; // for time factor, for motion blur
    vec3 p; // hit point
    vec3 normal;
    material *mat_ptr;
};

class hitable{
public:
    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &record) const = 0;
};

#endif //OVERTHEWEEKEND2_HITABLE_H
