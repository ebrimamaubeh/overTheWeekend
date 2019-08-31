#ifndef OVERTHEWEEKEND_RAY_H
#define OVERTHEWEEKEND_RAY_H

#include "vec3.h"

class ray{
public:
    ray(){}
    ray(const vec3 &o, const vec3 &d){
        _origin = o; _direction = d;
    }

    vec3 origin() const { return _origin; }
    vec3 direction() const { return _direction; }
    vec3 point_at_parameter(float t) const { return _origin + t*_direction; }

    vec3 _origin;
    vec3 _direction;
};

#endif //OVERTHEWEEKEND_RAY_H
