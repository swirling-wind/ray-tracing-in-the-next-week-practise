#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.h"
#include "ray.h"

class material;

struct hit_record
{
    point3 hit_point;
    vec3 normal_vec_of_hit;
    shared_ptr<material> hit_material;
    double t_of_ray{ 0.0 };
    double u{};
    double v{};
    bool is_front_face{ false };


    void set_face_normal(const ray& r, const vec3& outward_normal);
};

inline void hit_record::set_face_normal(const ray& r, const vec3& outward_normal)
{
	is_front_face = dot(r.direction(), outward_normal) < 0;
	normal_vec_of_hit = is_front_face ? outward_normal : -outward_normal;
}

class hittable
{
public:
    virtual ~hittable() = default;//
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif