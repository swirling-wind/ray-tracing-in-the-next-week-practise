#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "hittable.h"


class moving_sphere : public hittable
{
public:
	moving_sphere() = default;

	moving_sphere(
		point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
	{}

	bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

	[[nodiscard]] point3 center(double time) const;

public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;
};

inline point3 moving_sphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

inline bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t_of_ray = root;
    rec.hit_point = r.at(rec.t_of_ray);
    auto outward_normal = (rec.hit_point - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.hit_material = mat_ptr;

    return true;
}

#endif