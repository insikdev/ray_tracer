#include "pch.h"
#include "sphere.h"

Sphere::Sphere(float radius)
    : m_radius { radius }
{
}

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

    if (t0 < 0.0f && t1 < 0.0f) { // sphere is behind the ray.
        return false;
    }

    float t = glm::min(t0, t1);
    if (t < 0.0f) {
        t = glm::max(t0, t1);
    }

    glm::vec3 point = ray.origin + ray.direction * t;
    glm::vec3 normal = glm::normalize(point - m_center);
    glm::vec3 toEye = -ray.direction;

    if (glm::dot(toEye, normal) < 0) {
        std::cout << "invert\n";
        normal = -normal;
    }

    intersections.push_back({ t, point, normal, toEye, this });

    return true;
}
