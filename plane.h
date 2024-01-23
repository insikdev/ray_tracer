#pragma once

#include "base_object.h"

class Plane : public BaseObject {
public:
    Plane(const glm::vec3& normal, const glm::vec3& point);
    ~Plane() = default;

public:
    std::optional<Intersection> Intersect(const Ray& ray) override;

private:
    glm::vec3 m_normal;
    glm::vec3 m_point;
};
