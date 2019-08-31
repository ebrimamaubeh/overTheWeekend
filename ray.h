#ifndef OVERTHEWEEKEND_RAY_H
#define OVERTHEWEEKEND_RAY_H

#include "vec3.h"

class ray{
public:
    ray(){}
    ray(const vec3 &o, const vec3 &d, float t = 0.0){
        _origin = o; _direction = d; _time = t;
    }

    vec3 origin() const { return _origin; }
    vec3 direction() const { return _direction; }
    vec3 point_at_parameter(float t) const { return _origin + t*_direction; }
    float time() const { return _time; }

private:
    vec3 _origin;
    vec3 _direction;
    float _time;
};

#endif //OVERTHEWEEKEND_RAY_H
