#pragma once

class BaseObject;

class RayTracer {
public:
    RayTracer();
    ~RayTracer();

public:
    glm::vec3 TraceRay(const Ray& ray, uint8_t level);

private:
    std::optional<Intersection> FindNearestObject(const Ray& ray);

public: // world
    std::vector<BaseObject*> m_objects;
    std::vector<Light> m_lights { { glm::vec3 { 3.0f, 3.0f, -3.0f } } };
};
