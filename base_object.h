#pragma once

class BaseObject {
public:
    BaseObject() = default;
    virtual ~BaseObject() = default;

public:
    virtual std::optional<Intersection> Intersect(const Ray& ray) = 0;

public:
    Material m_material;
};
