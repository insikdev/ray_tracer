#pragma once

class BaseObject {
public:
    BaseObject() = default;
    virtual ~BaseObject() = default;

public:
    virtual bool Intersect(const Ray& ray, std::vector<Intersection>& intersections) = 0;
};
