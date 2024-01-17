#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere(0.4);
    obj1->m_center = glm::vec3(0.0f, 0.0f, 0.3f);
    obj1->m_material.diffuse = glm::vec3(0.9f, 0.5f, 0.2f);

    Sphere* obj2 = new Sphere(0.1f);
    obj2->m_center = glm::vec3(0.5f, 0.5f, 0.1f);
    obj2->m_material.diffuse = glm::vec3(0.8f, 0.3f, 0.6f);
    obj2->m_material.shininess = 64;

    Sphere* obj3 = new Sphere(0.4f);
    obj3->m_center = glm::vec3(-0.7f, -0.5f, 0.3f);
    obj3->m_material.diffuse = glm::vec3(0.1f, 0.1f, 0.6f);
    obj3->m_material.shininess = 256;

    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
    m_objects.push_back(obj3);
}

RayTracer::~RayTracer()
{
    for (BaseObject* obj : m_objects) {
        delete obj;
    }
}

glm::ivec3 RayTracer::TraceRay(const Ray& ray)
{
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

    color += Lighting(intersections[0]);

    return glm::ivec3(glm::clamp(color, 0.0f, 1.0f) * 255.0f);
}

glm::vec3 RayTracer::Lighting(const Intersection& intersection)
{
    Material& material = intersection.pObject->m_material;

    glm::vec3 color = material.ambient;

    glm::vec3 N = intersection.normal;
    glm::vec3 L = glm::normalize(m_lightPos - intersection.point);
    glm::vec3 R = glm::normalize(glm::reflect(-L, N));

    float diff = glm::max(glm::dot(N, L), 0.0f);
    float spec = glm::pow(glm::max(glm::dot(intersection.toEye, R), 0.0f), material.shininess);

    color += material.diffuse * diff;
    color += material.specular * spec;

    return color;
}
