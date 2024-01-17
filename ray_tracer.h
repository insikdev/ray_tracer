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
    bool TryFindFirstObject(const Ray& ray, Intersection* out);
    bool TryFindNearestObject(const Ray& ray, Intersection* out);
    glm::ivec3 ClampColor(const glm::vec3& color);

public: // world
    std::vector<BaseObject*> m_objects;
    glm::vec3 m_lightPos { -1.0f, 3.0f, -3.0f };
};
