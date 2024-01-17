#pragma once

#include "base_object.h"

class Triangle : public BaseObject {
public:
    Triangle(const glm::vec3& vv0, const glm::vec3& vv1, const glm::vec3& vv2);
    ~Triangle() = default;

public:
    bool Intersect(const Ray& ray, std::vector<Intersection>& intersections) override;

private:
    glm::vec3 m_v0;
    glm::vec3 m_v1;
    glm::vec3 m_v2;
    glm::vec3 m_normal;
};
