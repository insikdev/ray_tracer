#pragma once

#include "base_object.h"

class Sphere : public BaseObject {
public:
    Sphere() = default;
    ~Sphere() = default;

public:
    bool Intersect(const Ray& ray, std::vector<Intersection>& intersections) override;
    glm::vec3 GetSurfaceNormal(const glm::vec3& point) override;

public:
    glm::vec3 m_center { 0.0f };
    float m_radius { 1.0f };
};