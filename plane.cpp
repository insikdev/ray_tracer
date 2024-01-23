#include "pch.h"
#include "plane.h"

Plane::Plane(const glm::vec3& normal, const glm::vec3& point)
    : m_normal { glm::normalize(normal) }
    , m_point { point }
{
}

std::optional<Intersection> Plane::Intersect(const Ray& ray)
{
    if (glm::abs(glm::dot(m_normal, ray.direction)) < 1e-2f) { // when the plane and ray are parallel.
        return std::nullopt;
    }

    glm::vec3 v { m_point - ray.origin };

    if (glm::abs(glm::dot(m_normal, v)) < 1e-2f) { // when the ray origin is in the plane.
        return std::nullopt;
    }

    float t = glm::dot(v, m_normal) / glm::dot(ray.direction, m_normal);

    if (t < 0.0f) { // when the plane is behind the ray.
        return std::nullopt;
    }

    glm::vec3 point { ray.origin + ray.direction * t };

    return Intersection { t, point, m_normal, this };
}
