#pragma once

class BaseObject {
public:
    BaseObject() = default;
    virtual ~BaseObject() = default;

public:
    virtual bool Intersect(const Ray& ray, std::vector<Intersection>& intersections) = 0;
    virtual glm::vec3 GetSurfaceNormal(const glm::vec3& point) = 0;

public:
    Material m_material;
};
