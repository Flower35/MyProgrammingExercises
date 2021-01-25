////////////////////////////////////////////////////////////////
// "PhilosophersCore.cpp"
////////////////////////////////////////////////////////////////

#include <Lab08/Philosophers.h>


int32_t main()
{
    GLFWwindow * window;
    const GLFWvidmode * mode;

    /********************************/
    /* Inicjalizacja biblioteki GLFW */

    if (!glfwInit())
    {
        return 1;
    }

    /********************************/
    /* Utworzenie okna gry */

    window = glfwCreateWindow
    (
        DEFAULT_WINDOW_WIDTH,
        DEFAULT_WINDOW_HEIGHT,
        WINDOW_NAME,
        NULL,
        NULL
    );

    if (NULL == window)
    {
        glfwTerminate();
        return 1;
    }

    /********************************/
    /* Przygotowanie gry */

    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, Lab08::windowResizeCallback);

    if (NULL != (mode = glfwGetVideoMode(glfwGetPrimaryMonitor())))
    {
        glfwSetWindowPos
        (
            window,
            (mode->width  - DEFAULT_WINDOW_WIDTH)  / 2,
            (mode->height - DEFAULT_WINDOW_HEIGHT) / 2
        );
    }

    if (!Lab08::prepareTheApplication())
    {
        Lab08::endTheApplication();

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    Lab08::windowResizeCallback(NULL, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    /********************************/
    /* G³ówna pêtla aplikacji */

    while (!glfwWindowShouldClose(window))
    {
        Lab08::doTheDrawing(window);

        glfwPollEvents();
    }

    /********************************/
    /* Zamkniêcie okna aplikacji */

    Lab08::endTheApplication();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
