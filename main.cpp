#include "pch.h"
#include "app.h"

int main()
{
    const int width = 1280;
    const int height = 720;

    App app { width, height, "ray tracing" };
    app.Run();
    return 0;
}