#ifndef OVERTHEWEEKEND2_CAMERA_H
#define OVERTHEWEEKEND2_CAMERA_H

#include "ray.h"

vec3 random_in_unit_disk(){
    vec3 p;
    do{
        p = 2.0f * vec3(drand48(), drand48(), 0.f) - vec3(1.f, 1.f, 0.f);
    }while(dot(p, p) >= 1.0f);

    return p;
}

class camera{
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist){
        radius_lens = aperture/2;

        float theta = vfov*M_PI/180;
        float half_height = float(tan(theta/2));
        float half_width = aspect * half_height;

        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
        horizontal = 2.f * half_width * focus_dist * u;
        vertical = 2.f * half_height * focus_dist * v;
    }

    ray get_ray(float s, float t){
        vec3 rd = radius_lens*random_in_unit_disk();
        vec3 offset = u*rd.x() + v*rd.y();

        return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
    }

    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 u, v, w;
    float radius_lens;
};

#endif //OVERTHEWEEKEND2_CAMERA_H
