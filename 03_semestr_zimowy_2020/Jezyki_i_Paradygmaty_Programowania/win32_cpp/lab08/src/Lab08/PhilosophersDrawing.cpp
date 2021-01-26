////////////////////////////////////////////////////////////////
// "PhilosophersDrawing.cpp"
////////////////////////////////////////////////////////////////

#include <Lab08/Philosophers.h>


namespace Lab08
{

    ////////////////////////////////////////////////////////////////
    // Funkcja wizualizacji problemu jedz¹cych filozofów
    ////////////////////////////////////////////////////////////////
    void doTheDrawing(GLFWwindow * window)
    {
        int id;

        const float TABLE_RADIUS = 0.45f * SCREEN_HEIGHT;

        const Color3f TABLE_COLOR[2] =
        {
            Color3f(0.5f, 0.2f, 0),  // Light brown
            Color3f(0.2f, 0.1f, 0)   // Dark brown
        };

        /********************************/

        /* Wyczyszczenie ekranu (sceny) */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Narysowanie okr¹g³ego sto³u */

        drawCircle(TABLE_COLOR[0],        TABLE_RADIUS, 0, 0);
        drawCircle(TABLE_COLOR[1], 0.8f * TABLE_RADIUS, 0, 1.0f);
        drawCircle(TABLE_COLOR[0], 0.7f * TABLE_RADIUS, 0, 2.0f);

        /* Narysowanie talerzy i sztuæców */

        for (id = 0; id < PHILOSOPHERS_COUNT; id++)
        {
            if (PHILOSOPHER_STATE_ABSENT != PhilosopherState[id]())
            {
                drawPlate(id);
            }

            drawFork(id);
        }

        /* Zamiana buforów aktywnego okna */

        glfwSwapBuffers(window);
    }


    ////////////////////////////////////////////////////////////////
    // Narysowanie kó³ka lub gwiazdki na œrodku ekranu
    ////////////////////////////////////////////////////////////////
    void drawCircle(const Color3f & color, float outer_radius, float inner_radius, float z_order)
    {
        int n, m;
        float angle, x, y;

        float r[2] =
        {
            outer_radius,
            (0 == inner_radius) ? outer_radius : inner_radius
        };

        const int CORNERS = 24;
        const float ANGLE_STEP = 2.0f * M_PI / CORNERS;

        /********************************/

        glColor3f(color.red, color.green, color.blue);

        glBegin(GL_TRIANGLE_FAN);
        {
            glVertex3f(0, 0, z_order);

            glVertex3f(r[0], 0, z_order);

            angle = 2.0f * M_PI - ANGLE_STEP;

            for (n = 1; n < CORNERS; n++)
            {
                m = n % 2;
                x = r[m] * cosf(angle);
                y = r[m] * sinf(angle);

                glVertex3f(x, y, z_order);

                angle -= ANGLE_STEP;
            }

            glVertex3f(r[0], 0, z_order);
        }
        glEnd();
    }


    ////////////////////////////////////////////////////////////////
    // Narysowanie jednego talerza (reprezentacja stanu jednego filozofa)
    ////////////////////////////////////////////////////////////////
    void drawPlate(int32_t which)
    {
        const bool eating = PHILOSOPHER_STATE_EATING == PhilosopherState[which]();

        const float PLATE_PLACEMENT = - 0.3f * SCREEN_HEIGHT;

        const float PLATE_RADIUS = 0.25f * 0.5f * SCREEN_HEIGHT;

        const Color3f PLATE_COLOR[3] =
        {
            Color3f(0.5f, 0.5f, 0.5f),  // Light Grey
            Color3f(0.2f, 0.2f, 0.2f),  // Dark Grey
            Color3f(0.71f, 1.0f, 0)     // Light Green if I am eating
        };

        /********************************/

        glPushMatrix();

        /* Obrócenie wzglêdem œrodka sto³u */
        glRotatef(which * 360.0f / PHILOSOPHERS_COUNT, 0, 0, 1.0f);

        /* Wysuniêcie talerza wzglêdem œrodka sto³u */
        glTranslatef(0, PLATE_PLACEMENT, 0);

        drawCircle(PLATE_COLOR[0],        PLATE_RADIUS, 0, 3.0f);
        drawCircle(PLATE_COLOR[1], 0.8f * PLATE_RADIUS, 0, 4.0f);
        drawCircle(PLATE_COLOR[0], 0.7f * PLATE_RADIUS, 0, 5.0f);

        if (eating)
        {
            drawCircle(PLATE_COLOR[2], 0.6f * PLATE_RADIUS, 0.35f * PLATE_RADIUS, 6.0f);
        }

        glPopMatrix();
    }


    ////////////////////////////////////////////////////////////////
    // Narysowanie jednego sztuæca (reprezentacja stanu jednego widelca)
    ////////////////////////////////////////////////////////////////
    void drawFork(int32_t which)
    {
        const int placement_id = (int32_t) ForkState[which]();
        const int color_id     = (int32_t)(FORK_STATE_AVAILABLE != placement_id);

        const float FORK_ORDER  = 7.0f;

        const float FORK_HEIGHT = 0.3f * 0.5f * SCREEN_HEIGHT;
        const float FORK_WIDTH  = 0.5f * FORK_HEIGHT;

        const float FORK_ROTATION[3] =
        {
            0,        // Fork is available
            - 45.0f,  // Taken by the Left hand
            + 45.0f   // Taken by the Right hand
        };

        const Point2f FORK_TRANSLATION[3] =
        {
            Point2f(0,                  - 0.2f * SCREEN_HEIGHT),
            Point2f(+ 0.5 * FORK_WIDTH, - 0.3f * SCREEN_HEIGHT),
            Point2f(- 0.5 * FORK_WIDTH, - 0.3f * SCREEN_HEIGHT)
        };

        const Color3f FORK_COLOR[2] =
        {
            Color3f(1.0f, 1.0f, 0),    // Yellow if available
            Color3f(1.0f, 0.5f, 0.5f)  // Pink if taken
        };

        /********************************/

        glPushMatrix();

        /* Obrócenie wzglêdem œrodka sto³u */
        glRotatef
        (
            which * 360.0f / PHILOSOPHERS_COUNT - 0.5f * 360.f / PHILOSOPHERS_COUNT,
            0,    // X-axis
            0,    // Y-axis
            1.0f  // Z-axis
        );

        /* Wysuniêcie widelca wzglêdem œrodka sto³u */
        glTranslatef
        (
            FORK_TRANSLATION[placement_id].x,
            FORK_TRANSLATION[placement_id].y,
            0
        );

        /* Obrócenie widelca (w przypadku dosuniêcia go bli¿ej talerza) */
        glRotatef
        (
            FORK_ROTATION[placement_id],
            0,    // X-axis
            0,    // Y-axis
            1.0f  // Z-axis
        );

        glColor3f
        (
            FORK_COLOR[color_id].red,
            FORK_COLOR[color_id].green,
            FORK_COLOR[color_id].blue
        );

        /* Trzon widelca + œrodkowy z¹bek */
        glBegin(GL_TRIANGLE_STRIP);
        {
            glVertex3f(+ 0.1f * FORK_WIDTH, - 0.5f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.1f * FORK_WIDTH, - 0.5f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.1f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.1f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.1f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.1f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.1f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.1f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        }
        glEnd();

        /* Lewy z¹bek widelca */
        glBegin(GL_TRIANGLE_FAN);
        {
            glVertex3f(- 0.2f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.1f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.1f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.3f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.5f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(- 0.3f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        }
        glEnd();

        /* Prawy z¹bek widelca */
        glBegin(GL_TRIANGLE_FAN);
        {
            glVertex3f(+ 0.2f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.3f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.5f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.3f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.1f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
            glVertex3f(+ 0.1f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
        }
        glEnd();

        glPopMatrix();
    }


    ////////////////////////////////////////////////////////////////
    // Ustawienie wyœwietlania w uk³adzie ortogonalnym.
    // (rzut prostopad³y bez perspektywy trójwymiarowej, bez g³êbi)
    ////////////////////////////////////////////////////////////////
    void setOrthoView(double half_width, double half_height)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /* Mno¿enie macierzy projekcji przez now¹ macierz przekszta³cenia */
        glOrtho
        (
            - half_width,   // left
            + half_width,   // right
            - half_height,  // bottom
            + half_height,  // top
            - 10.0,         // near
            + 10.0          // far
        );

        /* Na koniec resetujemy macierz modelu */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }


    ////////////////////////////////////////////////////////////////
    // Funkcja wywo³ywana podczas zmiany rozmiaru okna aplikacji.
    ////////////////////////////////////////////////////////////////
    void windowResizeCallback(GLFWwindow * window, int width, int height)
    {
        const double ratio = ((0 != width) && (0 != height))
            ? ((double)width / height)
            : 1.0;

        double scene_y = 0.5 * SCREEN_HEIGHT;
        double scene_x = scene_y;

        if (width < height)
        {
            scene_y /= ratio;
        }
        else if (width > height)
        {
            scene_x *= ratio;
        }

        glViewport(0, 0, width, height);

        setOrthoView(scene_x, scene_y);
    }

}
