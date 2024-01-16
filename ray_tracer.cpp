#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere();

    m_objects.push_back(obj1);
}

RayTracer::~RayTracer()
{
    for (BaseObject* obj : m_objects) {
        delete obj;
    }
}

glm::vec3 RayTracer::CastRay(const glm::vec2& worldPos)
{
    glm::vec3 rayDir = glm::normalize(glm::vec3(worldPos, 0.0f) - m_rayOrigin);
    Ray ray { m_rayOrigin, rayDir };

    std::vector<Intersection> intersections;

    for (BaseObject* obj : m_objects) {
        obj->Intersect(ray, intersections);
    }

    if (intersections.empty()) {
        return glm::vec3(0.0f);
    }

    return glm::vec3(1.0f);
}
