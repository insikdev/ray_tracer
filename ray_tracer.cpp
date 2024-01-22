#include "pch.h"
#include "ray_tracer.h"
#include "sphere.h"
#include "triangle.h"

RayTracer::RayTracer()
{
    Sphere* obj1 = new Sphere(0.3f);
    obj1->m_center = glm::vec3(0.0f, 0.0f, 2.0f);
    obj1->m_material.diffuse = glm::vec3(0.3f, 0.9f, 0.8f);
    obj1->m_material.reflectance = 0.3f;

    Sphere* obj2 = new Sphere(0.1f);
    obj2->m_center = glm::vec3(0.5f, 0.5f, 1.1f);
    obj2->m_material.diffuse = glm::vec3(0.8f, 0.3f, 0.6f);
    obj2->m_material.shininess = 64;

    Sphere* obj3 = new Sphere(0.5f);
    obj3->m_center = glm::vec3(-0.7f, -0.5f, 2.0f);
    obj3->m_material.diffuse = glm::vec3(0.1f, 0.1f, 0.6f);
    obj3->m_material.shininess = 256;

    Sphere* obj7 = new Sphere(10.0f);
    obj7->m_center = glm::vec3(0.0f, -11.0f, 7.0f);
    obj7->m_material.diffuse = glm::vec3(0.8f);
    obj7->m_material.reflectance = 0.7f;

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

    obj4->m_material.ambient = glm::vec3(0.1f);
    obj5->m_material.ambient = glm::vec3(0.2f);
    obj6->m_material.ambient = glm::vec3(0.3f);

    m_objects.push_back(obj1);
    m_objects.push_back(obj2);
    m_objects.push_back(obj3);
    // m_objects.push_back(obj4);
    // m_objects.push_back(obj5);
    // m_objects.push_back(obj6);
    m_objects.push_back(obj7);
    // obj4->m_material.reflectance = 0.5f;
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

    glm::vec3 N { nearest->normal };
    glm::vec3 L { glm::normalize(m_lightPos - nearest->point) }; // point to light
    glm::vec3 V { -ray.direction }; // point to eye
    glm::vec3 R { glm::normalize(glm::reflect(-L, N)) }; // reflect incident light
    const Material& mat { nearest->pObject->m_material };

    Ray shadowRay { nearest->point + L * 1e-4f, L };
    std::optional<Intersection> shadow = FindNearestObject(shadowRay);

    if (shadow.has_value()) {
        float d = glm::length(m_lightPos - shadow->point);
        if (shadow->distance < d) {
            return glm::vec3(0.15f);
        }
    }

    glm::vec3 color { 0.0f };

    float diff = glm::max(glm::dot(N, L), 0.0f);
    float spec = static_cast<float>(glm::pow(glm::max(glm::dot(V, R), 0.0f), mat.shininess));
    color += (mat.ambient + mat.diffuse * diff + mat.specular * spec) * (1.0f - mat.reflectance);

    if (mat.reflectance) {
        glm::vec3 reflectDir = glm::normalize(glm::reflect(ray.direction, N)); // reflect ray
        Ray reflectRay { nearest->point + reflectDir * 1e-4f, reflectDir };
        color += TraceRay(reflectRay, level - 1) * mat.reflectance;
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
