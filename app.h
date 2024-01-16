#pragma once

class RayTracer;

class App {
public:
    App(uint32_t width, uint32_t height, std::string title);
    ~App();

public:
    void Run(void);

private:
    void Render(void);
    void HandleEvent(void);
    void WritePixel(uint32_t x, uint32_t y, const glm::ivec3& color);
    glm::vec2 TransformScreenToWorld(const glm::vec2& screenPos);

private:
    RayTracer* p_rayTracer;

private:
    uint32_t m_width;
    uint32_t m_height;
    float m_aspect;
    SDL_Window* m_window;
    SDL_PixelFormat* m_format;
    void* m_pixels;
    float m_pixelSize { 2.0f };
};
