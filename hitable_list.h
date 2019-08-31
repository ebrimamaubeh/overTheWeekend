#ifndef OVERTHEWEEKEND2_HITABLE_LIST_H
#define OVERTHEWEEKEND2_HITABLE_LIST_H

#include "hitable.h"
#include "moving_sphere.h"

class hitable_list : public hitable{

public:
    hitable_list(){}
    hitable_list(hitable **l, int n){
        list = l;
        size = n;
    }

    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;


    hitable **list;
    int size;
};


bool hitable_list::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = tmax;
    for(int i = 0; i < size; i++){
        if(list[i]->hit(r, tmin, closest_so_far, temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}


bool hitable_list::bounding_box(float t0, float t1, aabb &box) const{
    if(size == 0) return false;

    aabb temp_box;
    bool first_box = list[0]->bounding_box(t0, t1, temp_box);
    if(!first_box)
        return false;
    else
        box = temp_box;

    for(int i = 1; i < size; i++){
        if(list[0]->bounding_box(t0, t1, temp_box)){ // TODO: I think the 0 there is a bug.
            box = sorrounding_box(box, temp_box);
        }
        else{
            return false;
        }
    }

    return true;
}

#endif //OVERTHEWEEKEND2_HITABLE_LIST_H
