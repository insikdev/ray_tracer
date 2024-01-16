#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere();
    obj1->m_material.diffuse = glm::vec3(0.5f, 0.5f, 0.2f);

    m_objects.push_back(obj1);
}

RayTracer::~RayTracer()
{
    for (BaseObject* obj : m_objects) {
        delete obj;
    }
}

glm::ivec3 RayTracer::CastRay(const glm::vec2& worldPos)
{
    glm::vec3 rayDir = glm::normalize(glm::vec3(worldPos, 0.0f) - m_rayOrigin);
    Ray ray { m_rayOrigin, rayDir };

    std::vector<Intersection> intersections;

    for (BaseObject* obj : m_objects) {
        obj->Intersect(ray, intersections);
    }

    if (intersections.empty()) {
        return glm::vec3 { 0.0f };
    }

    std::sort(intersections.begin(), intersections.end(), [](const Intersection& a, const Intersection& b) {
        return a.distance < b.distance;
    });

    glm::vec3 color { 0.0f };

    BaseObject* obj = intersections[0].pObject;
    float t = intersections[0].distance;
    glm::vec3 point = m_rayOrigin + rayDir * t;

    glm::vec3 N = obj->GetSurfaceNormal(point);
    glm::vec3 L = glm::normalize(point - m_lightPos);
    glm::vec3 V = rayDir;
    glm::vec3 R = glm::normalize(glm::reflect(-L, N));

    float diff = glm::max(glm::dot(N, L), 0.0f);
    float spec = static_cast<float>(glm::pow(glm::max(glm::dot(V, R), 0.0f), obj->m_material.shininess));

    color += obj->m_material.ambient;
    color += obj->m_material.diffuse * diff;
    color += obj->m_material.specular * spec;

    return glm::ivec3(glm::clamp(color, 0.0f, 1.0f) * 255.0f);
}
