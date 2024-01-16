#pragma once

#include "base_object.h"

class Sphere : public BaseObject {
public:
    Sphere(float radius = 1.0f);
    ~Sphere() = default;

public:
    bool Intersect(const Ray& ray, std::vector<Intersection>& intersections) override;

public:
    glm::vec3 m_center { 0.0f };
    float m_radius;
};
