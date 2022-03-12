#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

// ReSharper disable once CppUnusedIncludeDirective
#include "rtweekend.h"

using std::sqrt;

class vec3
{
public:
	vec3() : e{ 0,0,0 } {}
	vec3(const double e0, const double e1, const double e2) : e{ e0, e1, e2 } {}

	[[nodiscard]] double x() const { return e[0]; }
	[[nodiscard]] double y() const { return e[1]; }
	[[nodiscard]] double z() const { return e[2]; }

	vec3  operator-() const { return vec3{ -e[0], -e[1], -e[2] }; }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	inline static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	vec3& operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	[[nodiscard]] double length() const
	{
		return sqrt(length_squared());
	}

	[[nodiscard]] double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	[[nodiscard]] bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

// ReSharper disable once CppRedundantAccessSpecifier
public:
	double e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << '(' << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ')';
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3{ u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2] };
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3{ u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2] };
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3{ u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2] };
}

inline vec3 operator*(double t, const vec3& v)
{
	return vec3{ t * v.e[0], t * v.e[1], t * v.e[2] };
}

inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
	return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3{ u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0] };
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

////////////////////

inline vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal)
{
	const vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;

	return -in_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& in_ray, const vec3& normal, const double etai_over_etat)
{
	// etai_over_etat == ¦Ç / ¦Ç'
	const double cos_theta = fmin(dot(-in_ray, normal), 1.0);
	const vec3 r_out_perpendicular = etai_over_etat * (in_ray + cos_theta * normal);
	const vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perpendicular.length_squared())) * normal;
	return r_out_perpendicular + r_out_parallel;
}

inline vec3 random_in_unit_disk()
{
	while (true)
	{
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

#endif
