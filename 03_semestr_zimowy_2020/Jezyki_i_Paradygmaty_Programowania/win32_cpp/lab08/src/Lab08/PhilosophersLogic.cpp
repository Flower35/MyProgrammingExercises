////////////////////////////////////////////////////////////////
// "PhilosophersLogic.cpp"
////////////////////////////////////////////////////////////////

#include <Lab08/Philosophers.h>


namespace Lab08
{

    ////////////////////////////////////////////////////////////////
    // Struktury pomocnicze
    ////////////////////////////////////////////////////////////////

    Point2f::Point2f(float _x, float _y)
    : x(_x), y(_y)
    {}

    Color3f::Color3f(float _r, float _g, float _b)
    : red(_r), green(_g), blue(_b)
    {}


    ////////////////////////////////////////////////////////////////
    // Klasa semafora zliczaj¹cego
    ////////////////////////////////////////////////////////////////

    Semaphore::Semaphore(size_t starting_value)
    : counter(starting_value)
    {}

    Semaphore::Semaphore(const Semaphore & other)
    : counter(other.counter)
    {}

    void Semaphore::operator () (size_t new_counter)
    {
        counter = new_counter;
    }

    void Semaphore::wait(int32_t opt_fork_id, int32_t new_state)
    {
        std::unique_lock<std::mutex> lock(mtx);

        while (counter <= 0)
        {
            convar.wait(lock);
        }

        counter--;

        if (opt_fork_id >= 0)
        {
            ForkState[opt_fork_id](new_state);
        }

        lock.unlock();
    }

    void Semaphore::signal(int32_t opt_fork_id)
    {
        mtx.lock();

        counter++;

        if (opt_fork_id >= 0)
        {
            ForkState[opt_fork_id](FORK_STATE_AVAILABLE);
        }

        mtx.unlock();

        convar.notify_all();
    }


    ////////////////////////////////////////////////////////////////
    // Klasa stanu obiektu wizualnego
    ////////////////////////////////////////////////////////////////

    VisualState::VisualState()
    : state(0)
    {}

    uint8_t VisualState::operator () (void) const
    {
        /* Wywo³anie obiektu bez parametru: "GET" */
        return state;
    }

    void VisualState::operator () (uint8_t new_state)
    {
        /* Wywo³anie obiektu z parametrem: "SET" */
        state = new_state;
    }


    ////////////////////////////////////////////////////////////////
    // Deklaracje zmiennych globalnych
    ////////////////////////////////////////////////////////////////

    std::thread PhilosopherThread[PHILOSOPHERS_COUNT];
    Semaphore AccessControl[1 + PHILOSOPHERS_COUNT];

    VisualState PhilosopherState[PHILOSOPHERS_COUNT];
    VisualState ForkState[PHILOSOPHERS_COUNT];


    ////////////////////////////////////////////////////////////////
    // Przygotowanie zmiennych u¿ywanych w aplikacji
    ////////////////////////////////////////////////////////////////
    bool prepareTheApplication()
    {
        int32_t id;

        std::srand(std::time(NULL));

        /********************************/

        glClearColor(0, 0.5f, 1.0f, 1.0f);  // B³êkit

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        /********************************/

        AccessControl[PHILOSOPHERS_COUNT](PHILOSOPHERS_COUNT - 1);

        /********************************/

        try
        {
            for (id = 0; id < PHILOSOPHERS_COUNT; id++)
            {
                PhilosopherThread[id] = std::thread(philosopherLogic, id);
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
        const int32_t x = 100;

        const int32_t ms = x *
        (
            std::rand() % ((ms_max - ms_min + 1) / x)
            + (ms_min / x)
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }


    ////////////////////////////////////////////////////////////////
    // W¹tek jednego z kilku filozofów
    ////////////////////////////////////////////////////////////////
    void philosopherLogic(int32_t id)
    {
        const int32_t left_fork  = id;
        const int32_t right_fork = (id + 1) % PHILOSOPHERS_COUNT;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        while (1)
        {
            AccessControl[PHILOSOPHERS_COUNT].wait();

            /* Filozof usiad³ przy stole i chwilkê rozmyœla */

            PhilosopherState[id](PHILOSOPHER_STATE_THINKING);
            philosopherWait(100, 2000);

            /* Filozof próbuj podnieœæ dwa widelce */

            AccessControl[left_fork].wait
            (
                left_fork,
                FORK_STATE_TAKEN_AS_LEFT
            );

            AccessControl[right_fork].wait
            (
                right_fork,
                FORK_STATE_TAKEN_AS_RIGHT
            );

            /* Delikwent udaje, ¿e wcina coœ smacznego */

            PhilosopherState[id](PHILOSOPHER_STATE_EATING);
            philosopherWait(1000, 3000);

            /* Filozof odk³ada widelce */

            AccessControl[left_fork].signal(left_fork);
            AccessControl[right_fork].signal(right_fork);

            /* Delikwent odchodzi od sto³u i udaje, ¿e nie jest na razie g³odny */

            AccessControl[PHILOSOPHERS_COUNT].signal();

            PhilosopherState[id](PHILOSOPHER_STATE_ABSENT);
            philosopherWait(100, 2000);
        }
    }

}
