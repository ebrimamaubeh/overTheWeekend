#ifndef OVERTHEWEEKEND2_PERLIN_H
#define OVERTHEWEEKEND2_PERLIN_H

#include "vec3.h"

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w){
    // to reduce the artifact in the interpolation.
    float uu = u*u*(3-2*u);
    float vv = v*v*(3-2*v);
    float ww = w*w*(3-2*w);
    float accum = 0;
    for (int i=0; i < 2; i++)
        for (int j=0; j < 2; j++)
            for (int k=0; k < 2; k++) {
                vec3 weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu))*
                         (j*vv + (1-j)*(1-vv))*
                         (k*ww + (1-k)*(1-ww))*dot(c[i][j][k], weight_v);
            }
    return accum;
}

class perlin{
public:
    float noice(const vec3 &p) const {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        vec3 c[2][2][2];
        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
        return perlin_interp(c, u, v, w);
    }

    static vec3 *ranvec;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};

static vec3* perlin_generate(){
    vec3 *p = new vec3[256];
    for(int i = 0; i < 256; i++){
        p[i] = unit_vector(vec3(float(-1 + 2*drand48()), float(-1 + 2*drand48()), float(-1 + 2*drand48())));
    }
    return p;
}

void permute(int *p, int n){
    for(int i = n; i > 0; i--){
        int rand_idx = int(drand48()* (i + 1));
        int tmp = p[i];
        p[i] = p[rand_idx];
        p[rand_idx] = tmp;
    }
}

static int* perlin_generate_perm(){
    int *p = new int[256];
    for(int i = 0; i < 256; i ++){
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

vec3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();

#endif //OVERTHEWEEKEND2_PERLIN_H
