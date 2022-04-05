#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "rtweekend.h"
#include "texture.h"

class ray;
struct hit_record;

class material
{
public:
	virtual ~material() = default;
	virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};

class lambertian final : public material
{
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    bool scatter ([[maybe_unused]]const ray& in_ray, const hit_record& rec, color& attenuation, ray& scattered)
	const override
    {
	    auto scatter_direction = rec.normal_vec_of_hit + random_unit_vector();

        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal_vec_of_hit;
        }
        scattered = ray(rec.hit_point, scatter_direction, in_ray.time());
        attenuation = albedo->value(rec.u, rec.v, rec.hit_point);
        return true;
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
    shared_ptr<texture> albedo;
};

class metal final : public material
{
public:
    explicit metal(const color& new_albedo, double new_fuzziness) : albedo(new_albedo), fuzziness(new_fuzziness < 1 ? new_fuzziness : 1) {}

    bool scatter(const ray& in_ray, const hit_record& record, color& attenuation, ray& scattered)
	const override
    {
	    const vec3 reflected = reflect(unit_vector(in_ray.direction()), record.normal_vec_of_hit);
        scattered = ray(record.hit_point, reflected + fuzziness * random_in_unit_sphere(), in_ray.time());
        attenuation = albedo;
        return (dot(scattered.direction(), record.normal_vec_of_hit) > 0);
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
    color albedo;
    double fuzziness;
};

class dielectric final : public material
{
public:
	explicit dielectric(double index_of_refraction) : refraction_index(index_of_refraction) {}

    bool scatter(const ray& in_ray, const hit_record& record, color& attenuation, ray& scattered)
	const override
    {
        const double refraction_ratio = record.is_front_face ? (1.0 / refraction_index) : refraction_index;
        const vec3 unit_direction = unit_vector(in_ray.direction());

        const double cos_theta = fmin(dot(-unit_direction, record.normal_vec_of_hit), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        const bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        vec3 direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, record.normal_vec_of_hit);
        else
            direction = refract(unit_direction, record.normal_vec_of_hit, refraction_ratio);
        
        scattered = ray(record.hit_point, direction, in_ray.time());
        attenuation = color(1.0, 1.0, 1.0); //glass surface absorbs nothing
        return true;
    }

// ReSharper disable once CppRedundantAccessSpecifier
public:
    double refraction_index; // Index of Refraction

private:
    static double reflectance(double cosine, double reflection_index)
    {
        // Use Schlick's approximation for reflectance.
        double r0 = (1 - reflection_index) / (1 + reflection_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};



#endif