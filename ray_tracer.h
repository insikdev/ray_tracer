#pragma once

class BaseObject;

class RayTracer {
public:
    RayTracer();
    ~RayTracer();

public:
    glm::ivec3 TraceRay(const Ray& ray);

private:
    glm::vec3 Lighting(const Intersection& intersection);

public: // world
    std::vector<BaseObject*> m_objects;
    glm::vec3 m_lightPos { 0.0f, 1.0f, -1.0f };
};
