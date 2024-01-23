#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere(1.0f);
    obj1->m_center = glm::vec3(0.0f, 1.0f, 3.0f);
    obj1->m_material.diffuse = glm::vec3(0.3f, 0.9f, 0.8f);
    obj1->m_material.transparency = 0.9f;
    obj1->m_material.reflective = 0.1f;

    Sphere* obj2 = new Sphere(0.8f);
    obj2->m_center = glm::vec3(2.0f, 1.5f, 5.5f);
    obj2->m_material.diffuse = glm::vec3(0.8f, 0.2f, 0.15f);

    Plane* plane1 = new Plane { glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f } };
    plane1->m_material.diffuse = glm::vec3 { 1.0f };
    plane1->m_material.reflective = 0.4f;

    Plane* plane2 = new Plane { glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 0.0f, 0.0f, 10.0f } };
    plane2->m_material.diffuse = glm::vec3 { 0.8f, 0.8f, 0.9f };

    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
    m_objects.push_back(plane1);
    m_objects.push_back(plane2);
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

    glm::vec3 surface = Lighting(ray, *nearest);
    glm::vec3 reflected = TraceReflect(ray, *nearest, level - 1);
    glm::vec3 refracted = TraceRefract(ray, *nearest, level - 1);

    if (nearest->pObject->m_material.reflective > 0.0f && nearest->pObject->m_material.transparency > 0.0f) {
        float reflectance = schlick(ray, *nearest);
        return surface + reflected * reflectance + refracted * (1.0f - reflectance);
    }

    return surface + reflected + refracted;
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

glm::vec3 RayTracer::Lighting(const Ray& ray, const Intersection& intersection)
{
    const Material& mat = intersection.pObject->m_material;

    glm::vec3 color { mat.ambient };

    for (const auto& light : m_lights) {
        glm::vec3 L { glm::normalize(light.position - intersection.point) }; // point to light
        float distanceToLight = glm::length(light.position - intersection.point);

        Ray shadowRay { intersection.point + L * 1e-4f, L };
        std::optional<Intersection> shadow = FindNearestObject(shadowRay);

        if (shadow.has_value() && shadow->distance < distanceToLight) {
            continue;
        }

        glm::vec3 R { glm::normalize(glm::reflect(-L, intersection.N)) }; // reflect light

        float diff = glm::max(glm::dot(intersection.N, L), 0.0f);
        float spec = static_cast<float>(glm::pow(glm::max(glm::dot(-ray.direction, R), 0.0f), mat.shininess));

        color += (mat.diffuse * diff + mat.specular * spec) * (1.0f - mat.reflective - mat.transparency);
    }

    return color;
}

glm::vec3 RayTracer::TraceReflect(const Ray& ray, const Intersection& intersection, uint8_t level)
{
    if (intersection.pObject->m_material.reflective) {
        glm::vec3 reflectDir = glm::normalize(glm::reflect(ray.direction, intersection.N)); // reflect ray
        Ray reflectRay { intersection.point + reflectDir * 1e-4f, reflectDir };

        return TraceRay(reflectRay, level) * intersection.pObject->m_material.reflective;
    }

    return glm::vec3 { 0.0f };
}

glm::vec3 RayTracer::TraceRefract(const Ray& ray, const Intersection& intersection, uint8_t level)
{
    if (intersection.pObject->m_material.transparency) {
        float eta;
        glm::vec3 normal;
        const float ior = 1.5f; // glass

        if (glm::dot(ray.direction, intersection.N) < 0.0f) {
            eta = ior;
            normal = intersection.N;
        } else {
            eta = 1.0f / ior;
            normal = -intersection.N;
        }

        const float cosTheta1 = glm::dot(-ray.direction, normal);
        const float sinTheta1 = glm::sqrt(1.0f - cosTheta1 * cosTheta1);
        const float sinTheta2 = sinTheta1 / eta;
        const float cosTheta2 = glm::sqrt(1.0f - sinTheta2 * sinTheta2);

        const glm::vec3 m = glm::normalize(glm::dot(normal, -ray.direction) * normal + ray.direction);
        const glm::vec3 a = m * sinTheta2;
        const glm::vec3 b = -normal * cosTheta2;
        const glm::vec3 refractedDir = glm::normalize(a + b);

        Ray refractionRay { intersection.point + refractedDir * 1e-4f, refractedDir };
        return TraceRay(refractionRay, level) * intersection.pObject->m_material.transparency;
    }

    return glm::vec3 { 0.0f };
}

float RayTracer::schlick(const Ray& ray, const Intersection& intersection)
{
    float eta;
    glm::vec3 normal;
    const float ior = 1.5f; // glass

    if (glm::dot(ray.direction, intersection.N) < 0.0f) {
        eta = ior;
        normal = intersection.N;
    } else {
        eta = 1.0f / ior;
        normal = -intersection.N;
    }

    const float cosTheta1 = glm::dot(-ray.direction, normal);
    const float r0 = ((ior - 1.0f) / (ior + 1.0f)) * ((ior - 1.0f) / (ior + 1.0f));

    return r0 + (1.0f - r0) * glm::pow(1.0f - cosTheta1, 5.0f);
}
