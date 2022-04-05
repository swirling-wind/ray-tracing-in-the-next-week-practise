#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() = default;
	ray(const point3& origin, const vec3& direction, double time = 0.0)
		: orig(origin), dir(direction), tm(time)
	{}

	[[nodiscard]] point3 origin() const { return orig; }
	[[nodiscard]] vec3 direction() const { return dir; }
	[[nodiscard]] double time() const { return tm; }

	[[nodiscard]] point3 at(const double t) const
	{
		return orig + t * dir;
	}

// ReSharper disable once CppRedundantAccessSpecifier
public:
	point3 orig;
	vec3 dir;
	double tm{};

};

#endif
