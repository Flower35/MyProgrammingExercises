////////////////////////////////////////////////////////////////
// "TetrisCore.cpp"
////////////////////////////////////////////////////////////////

#include <Tetris/Tetris.h>


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
        Tetris::endTheGame();

        glfwTerminate();
        return 1;
    }

    /********************************/
    /* Przygotowanie gry */

    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, Tetris::windowResizeCallback);

    if (NULL != (mode = glfwGetVideoMode(glfwGetPrimaryMonitor())))
    {
        glfwSetWindowPos
        (
            window,
            (mode->width  - DEFAULT_WINDOW_WIDTH)  / 2,
            (mode->height - DEFAULT_WINDOW_HEIGHT) / 2
        );
    }

    if (!Tetris::prepareTheGame())
    {
        Tetris::endTheGame();

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    Tetris::windowResizeCallback(NULL, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    /********************************/
    /* G³ówna pêtla aplikacji */

    while ((TETRIS_GAMESTATE_CLOSE != Tetris::GameState) && !glfwWindowShouldClose(window))
    {
        Tetris::doTheLogic(window);

        if (TETRIS_GAMESTATE_CLOSE != Tetris::GameState)
        {
            Tetris::doTheDrawing(window);
        }

        glfwPollEvents();
    }

    /********************************/
    /* Zamkniêcie gry */

    Tetris::endTheGame();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
