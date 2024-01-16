#include "pch.h"
#include "sphere.h"

bool Sphere::Intersect(const Ray& ray, std::vector<Intersection>& intersections)
{
    float b = glm::dot(ray.direction, ray.origin - m_center);
    float c = glm::dot(ray.origin - m_center, ray.origin - m_center) - m_radius * m_radius;

    float det = b * b - c;

    if (det < 0.0f) {
        return false;
    }

    float t0 = -b + glm::sqrt(det);
    float t1 = -b - glm::sqrt(det);

    intersections.push_back({ t0, this });
    intersections.push_back({ t1, this });

    return true;
}
