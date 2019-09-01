#ifndef OVERTHEWEEKEND2_AAREC_H
#define OVERTHEWEEKEND2_AAREC_H

#include "hitable.h"

class xy_rect : public hitable{
public:
    xy_rect() {}
    xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *m) : x0(_x0), x1(_x1),
              y0(_y0), y1(_y1), k(_k), mat(m)  {}

    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &record) const;

    virtual bool bounding_box(float t0, float t1, aabb &box) const{
        box = aabb(vec3(x0, y0, k - 0.0001f), vec3(x1, y1, k+0.0001f));
        return true;
    }

    float x0, x1;
    float y0, y1;
    float k;
    material *mat;
};

bool xy_rect::hit(const ray &r, float tmin, float tmax, hit_record &record) const {
    float t = (k - r.origin().z()) / r.direction().z();
    if(t < tmin || t > tmax)
        return false;

    float x = r.origin().x() + t*r.direction().x();
    float y = r.origin().y() + t*r.direction().y();
    if(x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    record.u = (x - x0)/(x1 - x0);
    record.v = (y - y0)/(y1 - y0);
    record.t = t;
    record.mat_ptr = mat;
    record.p = r.point_at_parameter(t);
    record.normal = vec3(0, 0, 1.f);
    return true;
}

#endif //OVERTHEWEEKEND2_AAREC_H
