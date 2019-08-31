#include <iostream>
#include <fstream>
#include <limits>

#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray &r, hitable *world, int dept){
    hit_record rec;
    float MAX_FLOAT = std::numeric_limits<float>::max();
    float tmin = 0.0001f; // ignore hits near zero;

    if(world->hit(r, tmin, MAX_FLOAT, rec)){
        vec3 attnuation;
        ray scattered;
        if(dept < 50 && rec.mat_ptr->scatter(r, rec, attnuation, scattered)){
            return (attnuation * color(scattered, world, dept + 1));
        }
        else{
            return vec3(0.f, 0.f, 0.f);
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0-t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
    }
}

hitable* random_scene(){
    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0, -1000.f, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
    int i = 1;
    for(int a = -11; a < 11; a++){
        for(int b = -11; b < 11; b++){
            float choose_mat = drand48();
            vec3 center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
            if((center - vec3(4, 0.2, 0)).length() > 0.9f){
                if(choose_mat < 0.8f){ // defuse.
                    float rsqr = drand48() * drand48();
                    list[i++] = new sphere(center, 0.2f, new lambertian(vec3(rsqr, rsqr, rsqr)));
                }
                else if(choose_mat < 0.95){ // metal.
                    float r0 = 0.5*(1 + drand48());
                    list[i++] = new sphere(center, 0.2f, new metal(vec3(r0, r0, r0), r0));
                }
                else{
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1.f, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4.0f, 1.f, 0), 1.0, new lambertian(vec3(.4f, .2f, .1f)));
    list[i++] = new sphere(vec3(4.0, 1.f, 0), 1.0, new metal(vec3(0.7f, 0.6f, 0.5f), 0.f));

    return new hitable_list(list, i);
}

int main(){
    std::ofstream file;
    file.open("../output.ppm");
    int nx = 400;
    int ny = 200;
    int ns = 200;
    file << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *world = random_scene();

    vec3 lookfrom (3.f, 3.f, 2.f);
    vec3 lookat = vec3(0.f, 0.f, -10.f);
    vec3 vup = vec3(0.f, 1.f, 0.f);
    float apeture = 2.f;
    float dist_to_focus = (lookfrom-lookat).length();

    camera cam(lookfrom, lookat, vup, 0, float(nx)/float(ny), apeture, dist_to_focus);

    for(int j = ny - 1; j >= 0; j--){
        for(int i = 0; i < nx; i++){

            vec3 col(0.f, 0.f, 0.f);
            for(int s = 0; s < ns; s++){
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0f);
                col = col + color(r, world, 0);
            }
            col /= float(ns);
            // gamma correction.
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            file << ir << " " << ig << " " << ib << std::endl;
        }
    }

    file.close();
}
