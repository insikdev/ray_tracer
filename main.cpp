#include "pch.h"
#include "app.h"

int main()
{
    const int width = 640;
    const int height = 480;

    App app { width, height, "ray tracing" };
    app.Run();
    return 0;
}