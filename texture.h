#ifndef OVERTHEWEEKEND2_TEXTURE_H
#define OVERTHEWEEKEND2_TEXTURE_H

#include "vec3.h"
#include "perlin.h"

class texture{
public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class constant_texture : public texture{
public:
    constant_texture() {}
    constant_texture(vec3 c) : color(c) {}
    virtual vec3 value(float u, float v, const vec3 &p) const {
        return color;
    }

    vec3 color;
};

class checker_texture : public texture{
public:
    checker_texture() {}
    checker_texture(texture *e, texture *o) : even(e), odd(o) {}

    virtual vec3 value(float u, float v, const vec3 &p) const{
        float sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
        if(sines < 0)
            return odd->value(u, v, p);
        return even->value(u, v, p);
    }

private:
    texture *even;
    texture *odd;
};

class noice_texture : public texture{
public:
    noice_texture() {}
    noice_texture(float sc) : scale(sc) {}
    virtual vec3 value(float u, float v, const vec3 &p) const {
        //return (vec3(1, 1, 1) * noice.noice(scale * p));
        return (vec3(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10* noice.turb(p))));
    }

    perlin noice;
    float scale = 1; // default.
};

#endif //OVERTHEWEEKEND2_TEXTURE_H
