#ifndef H_PHILOSOPHERS
#define H_PHILOSOPHERS

#define  _USE_MATH_DEFINES
#include <cmath>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <thread>
#include <mutex>
#include <condition_variable>

#define  GLFW_DLL
#include <GLFW/glfw3.h>

namespace Lab08
{

    ////////////////////////////////////////////////////////////////
    // Podstawowe definicje
    ////////////////////////////////////////////////////////////////

    #define SCREEN_WIDTH   640
    #define SCREEN_HEIGHT  480

    #define DEFAULT_WINDOW_WIDTH   SCREEN_WIDTH
    #define DEFAULT_WINDOW_HEIGHT  SCREEN_HEIGHT

    #define WINDOW_NAME  "Lab08: Philosophers"


    ////////////////////////////////////////////////////////////////
    // Struktury pomocnicze
    ////////////////////////////////////////////////////////////////

    struct Point2f
    {
        public:

            float x;
            float y;

            Point2f(float _x, float _y);
    };

    struct Color3f
    {
        public:

            float red;
            float green;
            float blue;

            Color3f(float _r, float _g, float _b);
    };


    ////////////////////////////////////////////////////////////////
    // Klasa semafora zliczaj¹cego
    ////////////////////////////////////////////////////////////////

    class Semaphore
    {
        private:

            size_t counter;
            std::mutex mtx;
            std::condition_variable convar;

            void operator () (size_t new_counter);

        public:

            Semaphore(size_t starting_value = 1);
            Semaphore(const Semaphore & other);

            void wait(int32_t opt_fork_id = (-1), int32_t new_state = 0);
            void signal(int32_t opt_fork_id = (-1));

        /* Semafor kamerdynera ma inn¹ wartoœæ pocz¹tkow¹! */
        friend bool prepareTheApplication();
    };


    ////////////////////////////////////////////////////////////////
    // Klasa stanu obiektu wizualnego
    ////////////////////////////////////////////////////////////////

    class VisualState
    {
        private:

            uint8_t state;

            void operator () (uint8_t new_state);

        public:

            VisualState();

            uint8_t operator () (void) const;

        /* Tylko filozofowie mog¹ bezpoœrednio zmieniaæ stan wizualny! */
        friend class Semaphore;
        friend void philosopherLogic(int32_t id);
    };


    ////////////////////////////////////////////////////////////////
    // Rozwi¹zywanie problemu ucztuj¹cych filozofów
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

    extern std::thread PhilosopherThread[PHILOSOPHERS_COUNT];
    extern Semaphore AccessControl[1 + PHILOSOPHERS_COUNT];

    extern VisualState PhilosopherState[PHILOSOPHERS_COUNT];
    extern VisualState ForkState[PHILOSOPHERS_COUNT];


    ////////////////////////////////////////////////////////////////
    // Funkcje - logika programu
    ////////////////////////////////////////////////////////////////

    bool prepareTheApplication();
    void endTheApplication();

    void philosopherWait(int32_t ms_min, int32_t ms_max);
    void philosopherLogic(int32_t id);


    ////////////////////////////////////////////////////////////////
    // Funkcje - wizualizacja programu
    ////////////////////////////////////////////////////////////////

    void doTheDrawing(GLFWwindow * window);

    void drawCircle(const Color3f & color, float outer_radius, float inner_radius, float z_order);
    void drawPlate(int32_t which);
    void drawFork(int32_t which);

    void setOrthoView(double half_width, double half_height);
    void windowResizeCallback(GLFWwindow * window, int width, int height);

}

#endif
