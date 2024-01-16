#pragma once

class Camera {
public:
    Camera() = default;
    ~Camera() = default;

public:
    Ray GetRay(const glm::vec3& worldPos);

public:
    glm::vec3 position { 0.0f, 0.0f, -2.0f };
    glm::vec3 at { 0.0f, 0.0f, 0.0f };
};
