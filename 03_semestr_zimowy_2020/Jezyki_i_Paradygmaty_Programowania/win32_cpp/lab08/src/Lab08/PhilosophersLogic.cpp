////////////////////////////////////////////////////////////////
// "PhilosophersLogic.cpp"
////////////////////////////////////////////////////////////////

#include <Lab08/Philosophers.h>


namespace Lab08
{

    ////////////////////////////////////////////////////////////////
    // Podstawowe definicje
    ////////////////////////////////////////////////////////////////

    COLOR3F::COLOR3F(float _r, float _g, float _b)
    : red(_r), green(_g), blue(_b)
    {}


    ////////////////////////////////////////////////////////////////
    // Deklaracje zmiennych globalnych
    ////////////////////////////////////////////////////////////////

    WINDOWPARAMS WindowParams;

    int32_t WindowWidth;
    int32_t WindowHeight;

    std::thread PhilosopherThread[PHILOSOPHERS_COUNT];

    uint8_t PhilosopherState[PHILOSOPHERS_COUNT];
    uint8_t ForkState[PHILOSOPHERS_COUNT];
    uint8_t TableRoom;


    ////////////////////////////////////////////////////////////////
    // Przygotowanie zmiennych u¿ywanych w aplikacji
    ////////////////////////////////////////////////////////////////
    bool prepareTheApplication()
    {
        int32_t id;

        /********************************/

        std::srand(std::time(NULL));

        /********************************/

        WindowParams.y     = 0.5 * SCREEN_HEIGHT;
        WindowParams.x     = WindowParams.y;
        WindowParams.ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

        WindowWidth  = DEFAULT_WINDOW_WIDTH;
        WindowHeight = DEFAULT_WINDOW_HEIGHT;

        /********************************/

        glClearColor(0, 0.5f, 1.0f, 1.0f);  // B³êkit

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        /********************************/

        std::memset(PhilosopherState, PHILOSOPHER_STATE_ABSENT, sizeof(uint8_t) * PHILOSOPHERS_COUNT);

        std::memset(ForkState, FORK_STATE_AVAILABLE, sizeof(uint8_t) * PHILOSOPHERS_COUNT);

        TableRoom = PHILOSOPHERS_COUNT - 1;

        /********************************/

        try
        {
            for (id = 0; id < PHILOSOPHERS_COUNT; id++)
            {
                PhilosopherThread[id] = std::thread(philosopherThread, id);
            }
        }
        catch (std::runtime_error)
        {
            return false;
        }

        /********************************/

        return true;
    }


    ////////////////////////////////////////////////////////////////
    // Zamkniêcie w¹tków przed wy³¹czeniem aplikacji
    ////////////////////////////////////////////////////////////////
    void endTheApplication()
    {
        for (int id = 0; id < PHILOSOPHERS_COUNT; id++)
        {
            PhilosopherThread[id].detach();
        }
    }


    ////////////////////////////////////////////////////////////////
    // Losowe oczekiwanie przez jednego z filozofów)
    ////////////////////////////////////////////////////////////////
    void philosopherWait(int32_t ms_min, int32_t ms_max)
    {
        const int32_t ms = (std::rand() % (ms_max - ms_min + 1)) + ms_min;

        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }


    ////////////////////////////////////////////////////////////////
    // W¹tek jednego z kilku filozofów
    ////////////////////////////////////////////////////////////////
    static void philosopherThread(int32_t id)
    {
        const int32_t left_fork  = id;
        const int32_t right_fork = (id + 1) % PHILOSOPHERS_COUNT;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        while (1)
        {
            if (TableRoom > 0)
            {
                TableRoom--;

                PhilosopherState[id] = PHILOSOPHER_STATE_THINKING;
                philosopherWait(100, 2000);

                while (FORK_STATE_AVAILABLE != ForkState[left_fork]);
                ForkState[left_fork] = FORK_STATE_TAKEN_AS_LEFT;

                while (FORK_STATE_AVAILABLE != ForkState[right_fork]);
                ForkState[right_fork] = FORK_STATE_TAKEN_AS_RIGHT;

                PhilosopherState[id] = PHILOSOPHER_STATE_EATING;
                philosopherWait(1000, 3000);

                ForkState[left_fork]  = FORK_STATE_AVAILABLE;
                ForkState[right_fork] = FORK_STATE_AVAILABLE;

                TableRoom++;
            }

            PhilosopherState[id] = PHILOSOPHER_STATE_ABSENT;
            philosopherWait(100, 2000);
        }
    }

}
