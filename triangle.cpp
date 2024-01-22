#include "pch.h"
#include "triangle.h"

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
    : m_v0 { v0 }
    , m_v1 { v1 }
    , m_v2 { v2 }
{
    glm::vec3 a = m_v1 - m_v0;
    glm::vec3 b = m_v2 - m_v0;
    m_normal = glm::normalize(glm::cross(a, b));
}

std::optional<Intersection> Triangle::Intersect(const Ray& ray)
{
    if (glm::abs(glm::dot(m_normal, ray.direction)) < 1e-2f) { // when the plane and ray are parallel.
        return std::nullopt;
    }

    glm::vec3 v { m_v0 - ray.origin };
    float t = glm::dot(v, m_normal) / glm::dot(ray.direction, m_normal);

    if (t < 0.0f) { // when the plane is behind the ray.
        return std::nullopt;
    }

    glm::vec3 point { ray.origin + ray.direction * t };

    glm::vec3 n0 = glm::cross(point - m_v2, m_v1 - m_v2);
    glm::vec3 n1 = glm::cross(point - m_v0, m_v2 - m_v0);
    glm::vec3 n2 = glm::cross(m_v1 - m_v0, point - m_v0);

    if (glm::dot(n0, m_normal) < 0.0f || glm::dot(n1, m_normal) < 0.0f || glm::dot(n2, m_normal) < 0.0f) { // when the intersection is outside the triangle.
        return std::nullopt;
    }

    glm::vec3 toEye = -ray.direction;
    bool isFront = glm::dot(m_normal, ray.direction) < 0.0f;

    return Intersection { t, point, m_normal, toEye, isFront, this };
}
