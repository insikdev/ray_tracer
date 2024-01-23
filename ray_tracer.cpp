#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere(0.3f);
    obj1->m_center = glm::vec3(0.0f, 0.0f, 2.0f);
    obj1->m_material.diffuse = glm::vec3(0.3f, 0.9f, 0.8f);

    Plane* plane = new Plane { glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f } };
    plane->m_material.diffuse = glm::vec3 { 0.9f, 0.1f, 0.1f };
    plane->m_material.reflective = 0.2f;

    m_objects.push_back(obj1);
    m_objects.push_back(plane);
}

RayTracer::~RayTracer()
{
    for (BaseObject* obj : m_objects) {
        delete obj;
    }
}

glm::vec3 RayTracer::TraceRay(const Ray& ray, uint8_t level)
{
    if (level == 0) {
        return glm::vec3 { 0.0f };
    }

    std::optional<Intersection> nearest = FindNearestObject(ray);

    if (!nearest.has_value()) {
        return glm::vec3 { 0.0f };
    }

    const Material& mat { nearest->pObject->m_material };
    glm::vec3 color { mat.ambient };
    const glm::vec3 V { -ray.direction };

    for (const auto& light : m_lights) {
        glm::vec3 L { glm::normalize(light.position - nearest->point) }; // point to light
        float distanceToLight = glm::length(light.position - nearest->point);

        Ray shadowRay { nearest->point + L * 1e-4f, L };
        std::optional<Intersection> shadow = FindNearestObject(shadowRay);

        if (shadow.has_value() && shadow->distance < distanceToLight) {
            continue;
        }

        glm::vec3 R { glm::normalize(glm::reflect(-L, nearest->N)) }; // reflect light

        float diff = glm::max(glm::dot(nearest->N, L), 0.0f);
        float spec = static_cast<float>(glm::pow(glm::max(glm::dot(V, R), 0.0f), mat.shininess));

        color += (mat.ambient + mat.diffuse * diff + mat.specular * spec) * (1.0f - mat.reflective - mat.transparency);
    }

    if (mat.reflective) {
        glm::vec3 reflectDir = glm::normalize(glm::reflect(-V, nearest->N)); // reflect V
        Ray reflectRay { nearest->point + reflectDir * 1e-4f, reflectDir };
        color += TraceRay(reflectRay, level - 1) * mat.reflective;
    }

    if (mat.transparency) {
    }

    return color;
}

std::optional<Intersection> RayTracer::FindNearestObject(const Ray& ray)
{
    Intersection nearest { std::numeric_limits<float>::max() };

    for (BaseObject* obj : m_objects) {
        const auto& intersection = obj->Intersect(ray);

        if (intersection.has_value() && intersection->distance < nearest.distance) {
            nearest = intersection.value();
        }
    }

    if (nearest.distance == std::numeric_limits<float>::max()) {
        return std::nullopt;
    }

    return nearest;
}
