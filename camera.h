#pragma once

class Screen;

class Camera {
public:
    Camera(const Screen* pScreen);
    ~Camera() = default;

public:
    Ray GetRay(const glm::vec2& screenPos);

public:
    float viewportWidth;
    float viewportHeight;
    glm::vec3 viewportOrigin;
    float focalLength { 1.0f };
    glm::vec3 dx;
    glm::vec3 dy;
    glm::vec3 eyePos { 0.0f };
    glm::vec3 eyeFront { 0.0f, 0.0f, 1.0f };

private:
    const Screen* p_screen;
};
