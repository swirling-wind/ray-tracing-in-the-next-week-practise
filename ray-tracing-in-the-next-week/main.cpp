#include "rtweekend.h"

#include <iostream>
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "texture.h"

color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record record;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color{ 0, 0, 0 };

    if (world.hit(r, 0.001, infinity, record))
    {
        ray scattered;
        color attenuation;
        if (record.hit_material->scatter(r, record, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color{ 0, 0, 0 };
    }
    const vec3 unit_direction = unit_vector(r.direction());
    const auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene()
{
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
	        const double choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


int main()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel =100;
    const int max_depth = 50;

    const hittable_list world = random_scene();

    // Camera
    const point3 lookfrom(13, 2, 3);
    const point3 lookat(0, 0, 0);
    const vec3 vup(0, 1, 0);
    const auto dist_to_focus = 10.0;
    const auto aperture = 0.1;

    const camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    // TODO: Only after completing the project should it experimentally apply OpenMP and Sobol sequence.
    std::ofstream output_image("output_image(0405_4_13).ppm");
    output_image << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScan-lines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
	            const double u = (i + random_double()) / (image_width - 1);
	            const double v = (j + random_double()) / (image_height - 1);
                ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(output_image, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}