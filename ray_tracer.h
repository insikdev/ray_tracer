#pragma once

class BaseObject;
#include "camera.h"

class RayTracer {
public:
    RayTracer();
    ~RayTracer();

public:
    glm::ivec3 CastRay(const glm::vec3& worldPos);

private:
    glm::vec3 Lighting(const Intersection& intersection, const Ray& ray);

public:
    Camera camera;
    std::vector<BaseObject*> m_objects;
    glm::vec3 m_lightPos { 0.0f, 1.0f, -1.0f };
};
