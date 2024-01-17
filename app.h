#pragma once

class Screen;
class Camera;
class RayTracer;

class App {
public:
    App(uint32_t width, uint32_t height, const std::string& title);
    ~App();

public:
    void Run(void);

private:
    void Render(void);
    void HandleEvent(void);

private:
    Screen* p_screen;
    Camera* p_camera;
    RayTracer* p_rayTracer;
};
