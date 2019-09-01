#ifndef OVERTHEWEEKEND2_SPHERE_H
#define OVERTHEWEEKEND2_SPHERE_H

#include "hitable.h"

class sphere : public hitable{
public:
    sphere(){}
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat(m) {}
    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &record) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;

    vec3 center;
    float radius;
    material *mat;
};

void get_sphere_uv(const vec3 &p, float &u, float &v){
    float pi = atan2(p.z(), p.x());
    float theta = asin(p.y());
    u = 1 - (pi + M_PI)/(2*M_PI);
    v = (theta + M_PI/2)/M_PI;
}

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
            get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }

        temp = (-b + float(sqrt(discriminant)))/a;
        if(temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
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

bool sphere::bounding_box(float t0, float t1, aabb &box) const {
    vec3 rv = vec3(radius, radius, radius);
    box = aabb(center - rv, center + rv);
    return true;
}

#endif //OVERTHEWEEKEND2_SPHERE_H
