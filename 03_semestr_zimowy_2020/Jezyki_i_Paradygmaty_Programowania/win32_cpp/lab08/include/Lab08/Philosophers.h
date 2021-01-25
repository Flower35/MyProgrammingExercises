#ifndef H_PHILOSOPHERS
#define H_PHILOSOPHERS

#define  _USE_MATH_DEFINES
#include <cmath>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <thread>

//// #define  GLFW_DLL
#include <GLFW/glfw3.h>

namespace Lab08
{

    ////////////////////////////////////////////////////////////////
    // Podstawowe definicje
    ////////////////////////////////////////////////////////////////

    #define SCREEN_WIDTH   640
    #define SCREEN_HEIGHT  480

    #define DEFAULT_WINDOW_WIDTH   640
    #define DEFAULT_WINDOW_HEIGHT  480

    #define WINDOW_NAME  "Lab08: Philosophers"


    struct WINDOWPARAMS
    {
        public:
            double x;
            double y;
            double ratio;
    };


    struct COLOR3F
    {
        public:

            float red;
            float green;
            float blue;

            COLOR3F(float _r, float _g, float _b);
    };


    ////////////////////////////////////////////////////////////////
    // Rozwi¹zywanie problemu g³odnych filozofów
    ////////////////////////////////////////////////////////////////

    #define PHILOSOPHERS_COUNT  5

    #define PHILOSOPHER_STATE_ABSENT    0
    #define PHILOSOPHER_STATE_THINKING  1
    #define PHILOSOPHER_STATE_EATING    2

    #define FORK_STATE_AVAILABLE       0
    #define FORK_STATE_TAKEN_AS_LEFT   1
    #define FORK_STATE_TAKEN_AS_RIGHT  2


    ////////////////////////////////////////////////////////////////
    // Zmienne globalne
    ////////////////////////////////////////////////////////////////

    extern WINDOWPARAMS WindowParams;

    extern int32_t WindowWidth;
    extern int32_t WindowHeight;

    extern std::thread PhilosopherThread[PHILOSOPHERS_COUNT];

    extern uint8_t PhilosopherState[PHILOSOPHERS_COUNT];
    extern uint8_t ForkState[PHILOSOPHERS_COUNT];
    extern uint8_t TableRoom;


    ////////////////////////////////////////////////////////////////
    // Funkcje - logika programu
    ////////////////////////////////////////////////////////////////

    bool prepareTheApplication();
    void endTheApplication();

    void philosopherWait(int32_t ms_min, int32_t ms_max);

    static void philosopherThread(int32_t id);


    ////////////////////////////////////////////////////////////////
    // Funkcje - wizualizacja programu
    ////////////////////////////////////////////////////////////////

    void doTheDrawing(GLFWwindow * window);

    void drawCircle(const COLOR3F & color, float outer_radius, float inner_radius, float z_order);
    void drawPlate(int32_t which);
    void drawFork(int32_t which);

    void setOrthoView(int width, int height);
    void windowResizeCallback(GLFWwindow * window, int width, int height);

}

#endif
