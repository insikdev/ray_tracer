#pragma once

class Screen {
public:
    Screen(uint32_t width, uint32_t height, const std::string& title);
    ~Screen();

public:
    void WritePixel(uint32_t x, uint32_t y, const glm::vec3& color);
    void UpdateSurface(void);
    void CaptureImageToBMP(void);

public: // getter
    uint32_t GetWidth(void) const { return m_width; }
    uint32_t GetHeight(void) const { return m_height; }
    float GetAspect(void) const { return m_aspect; }

private:
    const uint32_t m_width;
    const uint32_t m_height;
    const float m_aspect;

private:
    SDL_Window* m_window;
    SDL_Surface* m_surface;
    SDL_PixelFormat* m_format;
    void* m_pixels;
};
