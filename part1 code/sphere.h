#ifndef OVERTHEWEEKEND2_SPHERE_H
#define OVERTHEWEEKEND2_SPHERE_H

#include "hitable.h"

class sphere : public hitable{
public:
    sphere(){}
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat(m) {}
    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &record) const;

    vec3 center;
    float radius;
    material *mat;
};

bool sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b*b - a*c;

    if(discriminant > 0.0f){
        float temp = (-b - float(sqrt(discriminant)))/a;
        if(temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }

        temp = (-b + float(sqrt(discriminant)))/a;
        if(temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
    }

    return false;
}

// check later to see if you should add this here...
vec3 random_in_unit_sphere(){
    vec3 p;
    do{
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    }while(p.squared_length() >= 1.0f);

    return p;
}

#endif //OVERTHEWEEKEND2_SPHERE_H
