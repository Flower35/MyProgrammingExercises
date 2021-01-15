////////////////////////////////////////////////////////////////
// "TetrisDrawing.cpp"
////////////////////////////////////////////////////////////////

#include <Tetris/Tetris.h>


namespace Tetris
{

    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void doTheDrawing(GLFWwindow * window)
    {
        int a, x, y;
        float fx, fy, fz;

        /********************************/

        /* Wyczyszczenie ekranu (sceny) */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (TETRIS_GAMESTATE_MENU == GameState)
        {
            setPerspectiveView(WindowWidth, WindowHeight);
            glBindTexture(GL_TEXTURE_2D, BlockTextureId);
            glTranslatef(0, 0, 16 * BLOCK_WIDTH);
            drawAGrid(TETRIS_LOGO, TETRIS_LOGO_WIDTH, TETRIS_LOGO_HEIGHT);
            glTranslatef(0, 0, - 16 * BLOCK_WIDTH);
            glBindTexture(GL_TEXTURE_2D, 0);

            // (...) NIE DZIA£A PRZYWRÓCENIE POJEKCJI Z PERSPEKTYW¥!
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();

            setOrthoView(WindowWidth, WindowHeight);

            if (0 == GlobalPosition.x)
            {
                fx = 0.25f * SCREEN_WIDTH;
                fz = 0.5f * SCREEN_HEIGHT / 7;
                fy = 0 - fz;

                for (a = 0; a < 3; a++)
                {
                    if (a != GlobalPosition.y)
                    {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                    else
                    {
                        glColor3f(1.0f, 0, 0);
                    }

                    glBegin(GL_TRIANGLE_STRIP);
                    {
                        glVertex2f(- fx, fy);
                        glVertex2f(+ fx, fy);
                        fy -= fz;
                        glVertex2f(- fx, fy);
                        glVertex2f(+ fx, fy);
                        fy -= fz;
                    }
                    glEnd();
                }
            }
            else
            {
                glColor3f(1.0f, 1.0f, 0);

                fx = 0.25f * SCREEN_WIDTH;
                fz = 0.5f * SCREEN_HEIGHT / 5;
                fy = 0 - 3 * fz;

                glBegin(GL_TRIANGLE_STRIP);
                {
                    glVertex2f(- fx, fy);
                    glVertex2f(+ fx, fy);
                    fy -= fz;
                    glVertex2f(- fx, fy);
                    glVertex2f(+ fx, fy);
                }
                glEnd();
            }

            // (...) NIE DZIA£A PRZYWRÓCENIE POJEKCJI Z PERSPEKTYW¥!
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }
        else if (TETRIS_GAMESTATE_PLAYING == GameState)
        {
            /* Renderowanie krawêdzi planszy */

            glBindTexture(GL_TEXTURE_2D, 0);
            drawABorder();

            /* Renderowanie siatki bloków */

            glBindTexture(GL_TEXTURE_2D, BlockTextureId);

            drawAGrid(Grid, GRID_WIDTH, GRID_HEIGHT);

            /* Renderowanie tetromina spadaj¹cego */

            a = 0;

            for (y = 0; y < CurrentBlockSize; y++)
            {
                for (x = 0; x < CurrentBlockSize; x++)
                {
                    if (0 != CurrentBlock[a + x])
                    {
                        drawABlock
                        (
                            x * BLOCK_WIDTH + GlobalPosition.x * BLOCK_WIDTH + BLOCK_WIDTH / 2 - GRID_WIDTH / 2 * BLOCK_WIDTH,
                            (CurrentBlockSize - 1 - y) * BLOCK_WIDTH + GlobalPosition.y * BLOCK_WIDTH + 1.5f * BLOCK_WIDTH,
                            CurrentBlock[a + x]
                        );
                    }
                }

                a += 4;
            }
        }
        else if (TETRIS_GAMESTATE_LINECLEAR == GameState)
        {
            glBindTexture(GL_TEXTURE_2D, BlockTextureId);

            drawAGrid(Grid, GRID_WIDTH, GRID_HEIGHT);

            glBindTexture(GL_TEXTURE_2D, 0);

            // (...)
        }
        else if (TETRIS_GAMESTATE_TRYAGAIN == GameState)
        {
            // (...)
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        /* Zamiana buforów aktywnego okna */
        glfwSwapBuffers(window);
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void drawABorder()
    {
        int a, b;

        const int CORNERS = 12;
        const int FACES   =  8;

        const float colors[4][3] =
        {
            {0, 0, 0},
            {1.0f, 233 / 255.0f, 127 / 255.0f},
            {72 / 255.0f, 0, 1.0f},
            {0, 0, 0}
        };

        const float hw = GRID_WIDTH  * 0.5f * BLOCK_WIDTH;

        const float left   = - hw;
        const float right  = + hw;
        const float back   = + 1.5f * BLOCK_WIDTH;
        const float front  = - 1.5f * BLOCK_WIDTH;
        const float top    = + GRID_HEIGHT * BLOCK_WIDTH;
        const float bottom = 0;

        const float far_left   = left - BLOCK_WIDTH;
        const float far_right  = right + BLOCK_WIDTH;
        const float far_top    = top + BLOCK_WIDTH;
        const float far_bottom = bottom - BLOCK_WIDTH;

        const float corners[CORNERS][3] =
        {
            {left,  back,  top},             // [ 0]
            {right, back,  top},             // [ 1]
            {left,  front, top},             // [ 2]
            {right, front, top},             // [ 3]
            {left,  back,  bottom},          // [ 4]
            {right, back,  bottom},          // [ 5]
            {left,  front, bottom},          // [ 6]
            {right, front, bottom},          // [ 7]
            {far_left,  front, far_top},     // [ 8]
            {far_right, front, far_top},     // [ 9]
            {far_left,  front, far_bottom},  // [10]
            {far_right, front, far_bottom}   // [11]
        };

        const int indices[FACES][4] =
        {
            { 4,  5,  6,  7},  // bottom, inside
            { 6,  7, 10, 11},  // bottom, outside
            { 2,  3,  0,  1},  // top,    inside
            { 8,  9,  2,  3},  // top,    outside
            { 2,  0,  6,  4},  // left,   inside
            { 8,  2, 10,  6},  // left,   outside
            { 1,  3,  5,  7},  // right,  inside
            { 3,  9,  7, 11}   // right,  outside
        };

        /********************************/

        for (a = 0; a < FACES; a++)
        {
            glBegin(GL_TRIANGLE_STRIP);

            for (b = 0; b < 4; b++)
            {
                glColor3fv(colors[b]);
                glVertex3fv(corners[indices[a][b]]);
            }

            glEnd();
        }
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void drawABlock(float x, float z, uint8_t color)
    {
        int a;
        float r, g, b;

        const int CORNERS = 8;
        const int FACES   = 5;

        const float hw = BLOCK_WIDTH * 0.5f;

        const float left   = x - hw;
        const float right  = x + hw;
        const float back   = 0 + hw;
        const float front  = 0 - hw;
        const float top    = z + hw;
        const float bottom = z - hw;

        const float corners[CORNERS][3] =
        {
            {left,  back,  top},     // [0]
            {right, back,  top},     // [1]
            {left,  front, top},     // [2]
            {right, front, top},     // [3]
            {left,  back,  bottom},  // [4]
            {right, back,  bottom},  // [5]
            {left,  front, bottom},  // [6]
            {right, front, bottom}   // [7]
        };

        const int indices[FACES][4] =
        {
            {2, 3, 6, 7},  // front
            {0, 1, 2, 3},  // top
            {6, 7, 4, 5},  // bottom
            {0, 2, 4, 6},  // left
            {3, 1, 7, 5}   // right
        };

        /********************************/

        switch (color)
        {
            case 0x01:  // Red
            {
                r = 1.0f;
                g = 0.25f;
                b = 0.25f;
                break;
            }

            case 0x02:  // Green
            {
                r = 0.25f;
                g = 1.0f;
                b = 0.25f;
                break;
            }

            case 0x03:  // Blue
            {
                r = 0.25f;
                g = 0.25f;
                b = 1.0f;
                break;
            }

            case 0x04:  // Yellow
            {
                r = 1.0f;
                g = 1.0f;
                b = 0.25f;
                break;
            }

            default:
            {
                r = 1.0f;
                g = 1.0f;
                b = 1.0f;
            }
        }

        glColor3f(r, g, b);

        /********************************/

        for (a = 0; a < FACES; a++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                glTexCoord2f(0, 0);
                glVertex3fv(corners[indices[a][0]]);

                glTexCoord2f(1.0f, 0);
                glVertex3fv(corners[indices[a][1]]);

                glTexCoord2f(0, 1.0f);
                glVertex3fv(corners[indices[a][2]]);

                glTexCoord2f(1.0f, 1.0f);
                glVertex3fv(corners[indices[a][3]]);
            }
            glEnd();
        }
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void drawAGrid(const uint8_t * grid, int32_t width, int32_t height)
    {
        int32_t a = 0, x, y;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                if (0 != grid[a])
                {
                    drawABlock
                    (
                        BLOCK_WIDTH / 2 - width  / 2 * BLOCK_WIDTH + x * BLOCK_WIDTH,
                        height * BLOCK_WIDTH - BLOCK_WIDTH / 2 - y * BLOCK_WIDTH,
                        grid[a]
                    );
                }

                a++;
            }
        }
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void setOrthoView(int width, int height)
    {
        /* Uk³ad wspó³rzêdnych obserwatora */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        /* Mno¿enie macierzy projekcji przez now¹ macierz przekszta³cenia */
        glOrtho
        (
            - WindowParams.x,  // left
            WindowParams.x,    // right
            - WindowParams.y,  // bottom
            WindowParams.y,    // top
            - 1.0f,            // near
            1.0                // far
        );

        /* Na koniec resetujemy macierz modelu */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void setPerspectiveView(int width, int height)
    {
        /* Uk³ad wspó³rzêdnych obserwatora */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective
        (
            PERSPECTIVE_FOV,
            WindowParams.ratio,
            PERSPECTIVE_Z_NEAR,
            PERSPECTIVE_Z_FAR
        );

        /* Resetujemy macierz modelu */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* Zapamiêtaj przekszta³cenie wzglêdem kamery */
        /* ustawionej na planszê Tetrisa.             */
        gluLookAt
        (
            CAMERA_OBSERVER_X,  // oczy obserwatora
            CAMERA_OBSERVER_Y,
            CAMERA_OBSERVER_Z,
            CAMERA_TARGET_X,    // cel patrzenia
            CAMERA_TARGET_Y,
            CAMERA_TARGET_Z,
            0,                  // wektor wskazuj¹cy w górê [+Z]
            0,
            1.0
        );
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void windowResizeCallback(GLFWwindow * window, int width, int height)
    {
        WindowWidth  = width;
        WindowHeight = height;

        WindowParams.x = 0.5 * SCREEN_WIDTH;
        WindowParams.y = 0.5 * SCREEN_HEIGHT;

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

        setPerspectiveView(width, height);
    }

}
