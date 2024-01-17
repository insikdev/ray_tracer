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

glm::ivec3 RayTracer::TraceRay(const Ray& ray)
{
    Intersection nearest;

    if (!TryFindNearestObject(ray, &nearest)) {
        return glm::vec3 { 0.0f };
    }

    Intersection shadow;
    glm::vec3 shadowDir = glm::normalize(m_lightPos - nearest.point);
    Ray shadowRay { nearest.point + shadowDir * 1e-4f, shadowDir };

    if (TryFindFirstObject(shadowRay, &shadow)) {
        float d = glm::length(m_lightPos - shadow.point);
        if (shadow.distance < d) {
            glm::vec3 color = shadow.pObject->m_material.ambient;
            return ClampColor(color);
        }
    }

    glm::vec3 color = Lighting(nearest);
    return ClampColor(color);
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

bool RayTracer::TryFindFirstObject(const Ray& ray, Intersection* out)
{
    std::vector<Intersection> interscetions;

    for (BaseObject* obj : m_objects) {
        if (obj->Intersect(ray, interscetions)) {
            *out = interscetions[0];
            return true;
        }
    }

    return false;
}

bool RayTracer::TryFindNearestObject(const Ray& ray, Intersection* out)
{
    std::vector<Intersection> intersections;

    for (BaseObject* obj : m_objects) {
        obj->Intersect(ray, intersections);
    }

    if (intersections.empty()) {
        return false;
    }

    std::sort(intersections.begin(), intersections.end(), [](const Intersection& a, const Intersection& b) {
        return a.distance < b.distance;
    });

    *out = intersections[0];

    return true;
}

glm::ivec3 RayTracer::ClampColor(const glm::vec3& color)
{
    return glm::ivec3(glm::clamp(color, 0.0f, 1.0f) * 255.0f);
}
