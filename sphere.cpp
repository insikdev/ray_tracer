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

    float t = glm::min(t0, t1);
    if (t < 0.0f) {
        t = glm::max(t0, t1);
    }

    intersections.push_back({ t, this });

    return true;
}

glm::vec3 Sphere::GetSurfaceNormal(const glm::vec3& point)
{
    return glm::normalize(point - m_center);
}
