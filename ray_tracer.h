#pragma once

class BaseObject;

class RayTracer {
public:
    RayTracer();
    ~RayTracer();

public:
    glm::vec3 CastRay(const glm::vec2& worldPos);

public:
    std::vector<BaseObject*> m_objects;
    glm::vec3 m_rayOrigin { 0.0f, 0.0f, -5.0f };
};
