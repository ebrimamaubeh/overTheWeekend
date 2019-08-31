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
            return (vec3(0.f, 0.f, 0.f));
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0-t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
    }
}

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];

    //texture.
    texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));

    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    float rx = drand48()*drand48();
                    list[i++] = new sphere(center, 0.2, new lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                                           new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

hitable* two_perlin_shperes(){
    texture *pertext = new noice_texture(4);
    hitable **list = new hitable*[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));

    return new hitable_list(list, 2);
}

int main(){
    std::ofstream file;
    file.open("../output.ppm");
    int nx = 1200;
    int ny = 800;
    int ns = 10;
    file << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[5];
    float R = float(cos(M_PI/4.f));
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
    list[1] = new sphere(vec3(0,-100.5f,-1), 100, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45f, new dielectric(1.5));
    //hitable *world = new hitable_list(list,5);
    hitable *world = two_perlin_shperes();
    //world = random_scene();

    vec3 lookfrom (13.f, 2.f, 3.f);
    vec3 lookat = vec3(0.f, 0.f, 0.f);
    vec3 vup = vec3(0.f, 1.f, 0.f);
    float apeture = 0.f;
    float dist_to_focus = 10.0f;

    camera cam(lookfrom, lookat, vup, 20, float(nx)/float(ny), apeture, dist_to_focus, 0.f, 1.f);

    for(int j = ny - 1; j >= 0; j--){
        for(int i = 0; i < nx; i++){

            vec3 col(0.f, 0.f, 0.f);
            for(int s = 0; s < ns; s++){
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0f);
                vec3 test = color(r, world, 0);
                col = col + test;
            }
            col /= float(ns);
            // gamma correction.
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            if(ir < 0 || ig< 0 || ib < 0){
                std::cout << ir << " " << ig << " " << ib << std::endl;
            }

            file << ir << " " << ig << " " << ib << std::endl;
        }
    }

    file.close();
}
