#ifndef OVERTHEWEEKEND2_MOVING_SPHERE_H
#define OVERTHEWEEKEND2_MOVING_SPHERE_H

#include "material.h"

class moving_sphere : public hitable{
public:
    moving_sphere() {}
    moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) : center0(cen0),
        center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}

    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;
    vec3 center(float time) const;

    vec3 center0, center1;
    float time0, time1;
    float radius;
    material *mat_ptr;
};

vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0)/(time1-time0))*(center1-center0);
}


bool moving_sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;

    if(discriminant > 0){
        float temp = (-b - sqrt(discriminant))/a;
        if(temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant))/a;
        if(temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb &box) const {
    throw "bounding_box of moving sphere not implemented\n";
}

aabb sorrounding_box(aabb box0, aabb box1) {
    vec3 small(ffmin(box0.min().x(), box1.min().x()),
               ffmin(box0.min().y(), box1.min().y()),
               ffmin(box0.min().z(), box1.min().z()));

    vec3 big (ffmax(box0.max().x(), box1.max().x()),
              ffmax(box0.max().y(), box1.max().y()),
              ffmax(box0.max().z(), box1.max().z()));

    return (aabb(small, big));
}

#endif //OVERTHEWEEKEND2_MOVING_SPHERE_H
