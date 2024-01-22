#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"
#include "triangle.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere(0.4f);
    obj1->m_center = glm::vec3(0.0f, 0.0f, 1.7f);
    obj1->m_material.diffuse = glm::vec3(0.9f, 0.5f, 0.2f);

    Sphere* obj2 = new Sphere(0.1f);
    obj2->m_center = glm::vec3(0.5f, 0.5f, 1.1f);
    obj2->m_material.diffuse = glm::vec3(0.8f, 0.3f, 0.6f);
    obj2->m_material.shininess = 64;

    Sphere* obj3 = new Sphere(0.5f);
    obj3->m_center = glm::vec3(-0.7f, -0.5f, 2.0f);
    obj3->m_material.diffuse = glm::vec3(0.1f, 0.1f, 0.6f);
    obj3->m_material.shininess = 256;

    Triangle* obj4 = new Triangle {
        glm::vec3 { -2.0f, -2.0f, 1.0f },
        glm::vec3 { 2.0f, -2.0f, 1.0f },
        glm::vec3 { 0.0f, 0.0f, 3.0f },
    };

    Triangle* obj5 = new Triangle {
        glm::vec3 { -2.0f, -2.0f, 1.0f },
        glm::vec3 { 0.0f, 0.0f, 3.0f },
        glm::vec3 { 0.0f, 10.0f, 3.0f },
    };

    Triangle* obj6 = new Triangle {
        glm::vec3 { 0.0f, 10.0f, 3.0f },
        glm::vec3 { 0.0f, 0.0f, 3.0f },
        glm::vec3 { 2.0f, -2.0f, 1.0f },
    };

    obj4->m_material.ambient = glm::vec3(0.2f);
    obj5->m_material.ambient = glm::vec3(0.3f);
    obj6->m_material.ambient = glm::vec3(0.4f);
    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
    m_objects.push_back(obj3);
    m_objects.push_back(obj4);
    m_objects.push_back(obj5);
    m_objects.push_back(obj6);
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

    glm::vec3 shadowDir = glm::normalize(m_lightPos - nearest->point);
    Ray shadowRay { nearest->point + shadowDir * 1e-4f, shadowDir };

    std::optional<Intersection> shadow = FindFirstObject(shadowRay);

    if (shadow.has_value()) {
        float d = glm::length(m_lightPos - shadow->point);
        if (shadow->distance < d) {
            return glm::vec3(0.1f);
            // return shadow.pObject->m_material.ambient;
        }
    }

    glm::vec3 color = Lighting(nearest.value());
    return color;
}

glm::vec3 RayTracer::Lighting(const Intersection& intersection)
{
    Material& material = intersection.pObject->m_material;

    glm::vec3 N = intersection.normal;
    glm::vec3 L = glm::normalize(m_lightPos - intersection.point);
    glm::vec3 R = glm::normalize(glm::reflect(-L, N));

    float diff = glm::max(glm::dot(N, L), 0.0f);
    float spec = glm::pow(glm::max(glm::dot(intersection.toEye, R), 0.0f), material.shininess);

    return material.ambient + material.diffuse * diff + material.specular * spec;
}

std::optional<Intersection> RayTracer::FindFirstObject(const Ray& ray)
{
    for (BaseObject* obj : m_objects) {
        const auto& intersection = obj->Intersect(ray);

        if (intersection.has_value()) {
            return intersection.value();
        }
    }

    return std::nullopt;
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
