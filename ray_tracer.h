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
    glm::vec3 Lighting(const Ray& ray, const Intersection& intersection);
    glm::vec3 TraceReflect(const Ray& ray, const Intersection& intersection, uint8_t level);
    glm::vec3 TraceRefract(const Ray& ray, const Intersection& intersection, uint8_t level);
    float schlick(const Ray& ray, const Intersection& intersection);

public: // world
    std::vector<BaseObject*> m_objects;
    std::vector<Light> m_lights { { glm::vec3 { 3.0f, 3.0f, -3.0f } } };
};
