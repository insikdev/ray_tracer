#include "pch.h"
#include "sphere.h"

Sphere::Sphere(float radius)
    : m_radius { radius }
{
}

bool Sphere::Intersect(const Ray& ray, std::vector<Intersection>& intersections)
{
    glm::vec3 oc = ray.origin - m_center;
    float b = glm::dot(ray.direction, oc);
    float c = glm::dot(oc, oc) - m_radius * m_radius;

    float discriminant = b * b - c;

    if (discriminant < 0.0f) {
        return false;
    }

    float t0 = -b + glm::sqrt(discriminant);
    float t1 = -b - glm::sqrt(discriminant);

    if (t0 < 0.0f && t1 < 0.0f) { // sphere is behind the ray.
        return false;
    }

    float t = glm::min(t0, t1);
    if (t < 0.0f) {
        t = glm::max(t0, t1);
    }

    glm::vec3 point = ray.origin + ray.direction * t;
    glm::vec3 normal = (point - m_center) / m_radius;
    glm::vec3 toEye = -ray.direction;
    bool isFront { true };

    if (glm::dot(toEye, normal) < 0.0f) {
        isFront = false;
        normal = -normal;
    }

    intersections.push_back({ t, point, normal, toEye, isFront, this });

    return true;
}
