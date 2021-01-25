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

        const COLOR3F TABLE_COLOR[2] =
        {
            COLOR3F(0.5f, 0.2f, 0),  // Light brown
            COLOR3F(0.2f, 0.1f, 0)   // Dark brown
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
            if (PHILOSOPHER_STATE_ABSENT != PhilosopherState[id])
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
    void drawCircle(const COLOR3F & color, float outer_radius, float inner_radius, float z_order)
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
        const float PLATE_PLACEMENT = - 0.3f * SCREEN_HEIGHT;

        const float PLATE_RADIUS = 0.25f * 0.5f * SCREEN_HEIGHT;

        const COLOR3F PLATE_COLOR[3] =
        {
            COLOR3F(0.5f, 0.5f, 0.5f),  // Light Grey
            COLOR3F(0.2f, 0.2f, 0.2f),  // Dark Grey
            COLOR3F(0.71f, 1.0f, 0)     // Light Green if I am eating
        };

        /********************************/

        glPushMatrix();

        glRotatef(which * 360.0f / PHILOSOPHERS_COUNT, 0, 0, 1.0f);
        glTranslatef(0, PLATE_PLACEMENT, 0);

        drawCircle(PLATE_COLOR[0],        PLATE_RADIUS, 0, 3.0f);
        drawCircle(PLATE_COLOR[1], 0.8f * PLATE_RADIUS, 0, 4.0f);
        drawCircle(PLATE_COLOR[0], 0.7f * PLATE_RADIUS, 0, 5.0f);

        if (PHILOSOPHER_STATE_EATING == PhilosopherState[which])
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
        int color_id     = (int32_t)(FORK_STATE_AVAILABLE != ForkState[which]);
        int placement_id = (int32_t)(ForkState[which]);

        const float FORK_ORDER  = 7.0f;

        const float FORK_HEIGHT = 0.3f * 0.5f * SCREEN_HEIGHT;
        const float FORK_WIDTH  = 0.5f * FORK_HEIGHT;

        const float FORK_ROTATION[3] =
        {
            0,        // Fork is available
            - 45.0f,  // Taken by the Left hand
            + 45.0f   // Taken by the Right hand
        };

        const float FORK_TRANSLATION[3][2] =
        {
            {0,                  - 0.2f * SCREEN_HEIGHT},
            {+ 0.5 * FORK_WIDTH, - 0.3f * SCREEN_HEIGHT},
            {- 0.5 * FORK_WIDTH, - 0.3f * SCREEN_HEIGHT}
        };

        const COLOR3F FORK_COLOR[2] =
        {
            COLOR3F(1.0f, 1.0f, 0),    // Yellow if available
            COLOR3F(1.0f, 0.5f, 0.5f)  // Pink if taken
        };

        /********************************/

        glPushMatrix();

        glRotatef(which * 360.0f / PHILOSOPHERS_COUNT - 0.5f * 360.f / PHILOSOPHERS_COUNT, 0, 0, 1.0f);
        glTranslatef(FORK_TRANSLATION[placement_id][0], FORK_TRANSLATION[placement_id][1], 0);
        glRotatef(FORK_ROTATION[placement_id], 0, 0, 1.0f);

        glColor3f(FORK_COLOR[color_id].red, FORK_COLOR[color_id].green, FORK_COLOR[color_id].blue);

        //@ glBegin(GL_LINE_LOOP);
        //@ {
        //@     glVertex3f(- 0.5f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.3f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.2f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.1f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.1f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.1f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.1f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.2f * FORK_WIDTH, + 0.3f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.3f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.5f * FORK_WIDTH, + 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@
        //@     glVertex3f(+ 0.3f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.1f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(+ 0.1f * FORK_WIDTH, - 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.1f * FORK_WIDTH, - 0.5f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.1f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
        //@     glVertex3f(- 0.3f * FORK_WIDTH, + 0.2f * FORK_HEIGHT, FORK_ORDER);
        //@ }
        //@ glEnd();

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
    void setOrthoView(int width, int height)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /* Mno¿enie macierzy projekcji przez now¹ macierz przekszta³cenia */
        glOrtho
        (
            - WindowParams.x,  // left
            WindowParams.x,    // right
            - WindowParams.y,  // bottom
            WindowParams.y,    // top
            - 10.0,            // near
            10.0               // far
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
        WindowWidth  = width;
        WindowHeight = height;

        WindowParams.y = 0.5 * SCREEN_HEIGHT;
        WindowParams.x = WindowParams.y;

        WindowParams.ratio = ((0 != width) && (0 != height))
            ? ((double)width / height)
            : 1.0;

        if (width < height)
        {
            WindowParams.y /= WindowParams.ratio;
        }
        else if (width > height)
        {
            WindowParams.x *= WindowParams.ratio;
        }

        glViewport(0, 0, width, height);

        setOrthoView(width, height);
    }

}
