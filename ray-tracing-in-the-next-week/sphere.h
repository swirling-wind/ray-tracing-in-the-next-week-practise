#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere final : public hittable
{
public:
	sphere() = default;
	sphere(const point3 cen, const double r, shared_ptr<material> m)
		: center(cen), radius(r), object_material(std::move(m))
	{}

	bool hit(
		const ray& r, double min_t_of_ray, double max_t_of_ray, hit_record& record
	) const override;

	bool bounding_box(double time0, double time1, aabb& output_box) const override;

private:
	static void get_sphere_uv(const point3& p, double& u, double& v)
	{
		// p: a given point on the sphere of radius one, centered at the origin.
		// u: returned value [0,1] of angle around the Y axis from X=-1.
		// v: returned value [0,1] of angle from Y=-1 to Y=+1.
		//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
		//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
		//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50> 
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}

// ReSharper disable once CppRedundantAccessSpecifier
public:
	point3 center;
	double radius{};
	shared_ptr<material> object_material;
};

inline bool sphere::hit(const ray& r, double min_t_of_ray, double max_t_of_ray, hit_record& record) const
{
	const vec3 oc = r.origin() - center;
	const double half_b = dot(oc, r.direction());
	const double a = r.direction().length_squared();
	const double c = oc.length_squared() - radius * radius;

	const double discriminant = half_b * half_b - a * c;

	if (discriminant < 0)
		return false;
	const double sqrt_delta = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrt_delta) / a;
	if (root < min_t_of_ray || max_t_of_ray < root)
	{
		root = (-half_b + sqrt_delta) / a;
		if (root < min_t_of_ray || max_t_of_ray < root)
		{
			return false;
		}
	}

	record.t_of_ray = root;
	record.hit_point = r.at(record.t_of_ray);

	const vec3 outward_normal = (record.hit_point - center) / radius;
	record.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, record.u, record.v);
	record.hit_material = object_material;

	return true;
}

inline bool sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
	output_box = aabb(
		center - vec3{ radius, radius, radius },
		center + vec3{ radius, radius, radius });
	return true;
}

#endif