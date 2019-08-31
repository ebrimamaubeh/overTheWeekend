#ifndef OVERTHEWEEKEND2_MATERIAL_H
#define OVERTHEWEEKEND2_MATERIAL_H

#include "hitable.h"
#include "sphere.h"
#include "texture.h"

class material{
public:
    virtual bool scatter(const ray &in_ray, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material{
public:
    lambertian(texture *a) : albedo(a) {}
    virtual bool scatter(const ray &in_ray, const hit_record &rec, vec3 &attenuation, ray &scattered) const{
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, in_ray.time());
        attenuation = albedo->value(0, 0, rec.p);
        return true;
    }

    texture *albedo;
};

vec3 reflect(const vec3 &v, const vec3 &n){
    return (v - 2*dot(v, n)*n);
}

class metal : public material{
public:
    metal(vec3 a, float f) : albedo(a) {
        if(f < 1) fuzz = f;
        else fuzz = 1;
    }
    virtual bool scatter(const ray &in_ray, const hit_record &rec, vec3 &attenuation, ray &scattered) const{
        vec3 reflected = reflect(unit_vector(in_ray.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    float fuzz;
};

bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted){
    vec3 uv = unit_vector(v);
    float cos1 = dot(uv, n); // cos of the angle.. incident ray.
    double discriminant = 1.0f - (ni_over_nt*ni_over_nt)*(1.0f - cos1*cos1);
    if(discriminant > 0){
        refracted = ni_over_nt*(uv - n*cos1) - n*float(sqrt(discriminant));
        return true;
    }
    return false;

}

float schlick(float cosine, float ref_idx){
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return (r0 + (1 - r0)*float(pow(1-cosine, 5)));
}


class dielectric : public material{
public:
    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(const ray &in_ray, const hit_record &rec, vec3 &attnuation, ray &scattered) const{
        vec3 outward_normal;
        vec3 refracted;
        attnuation = vec3(1.f, 1.f, 1.f);
        float ni_over_nt;
        float reflect_prob;
        float cosine;

        vec3 reflected = reflect(in_ray.direction(), rec.normal);
        if(dot(reflected, rec.normal) > 0){
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx*dot(in_ray.direction(), rec.normal) / in_ray.direction().length();
        }
        else{
            outward_normal = rec.normal;
            ni_over_nt = 1.0f/ref_idx;
            cosine = -dot(in_ray.direction(), rec.normal)/in_ray.direction().length();
        }

        if(refract(in_ray.direction(), outward_normal, ni_over_nt, refracted)){
            reflect_prob = schlick(cosine, ref_idx);
        }
        else{
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.f;
        }

        if(drand48() < reflect_prob){
            scattered = ray(rec.p, reflected);
        }
        else{
            scattered = ray(rec.p, refracted);
        }

        return true;
    }


    float ref_idx;
};

#endif //OVERTHEWEEKEND2_MATERIAL_H
