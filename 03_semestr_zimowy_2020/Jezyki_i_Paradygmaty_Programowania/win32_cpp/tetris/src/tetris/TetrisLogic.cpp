////////////////////////////////////////////////////////////////
// "TetrisLogic.cpp"
////////////////////////////////////////////////////////////////

#include <Tetris/Tetris.h>


namespace Tetris
{

    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void doTheLogic(GLFWwindow * window)
    {
        int32_t a, b;
        float frames;
        int32_t collision;

        const size_t GRID_SIZE = GRID_WIDTH * GRID_HEIGHT;

        const int KEYBOARD_MAP[TETRIS_KEYS] =
        {
            GLFW_KEY_UP,     // TETRIS_KEY_UP
            GLFW_KEY_LEFT,   // TETRIS_KEY_LEFT
            GLFW_KEY_RIGHT,  // TETRIS_KEY_RIGHT
            GLFW_KEY_DOWN,   // TETRIS_KEY_DOWN
            GLFW_KEY_ESCAPE  // TETRIS_KEY_PAUSE
        };

        /********************************/

        frames = glfwGetTime() * FRAMES_PER_SECOND;
        DeltaFrames = frames - PreviousFrames;
        PreviousFrames = frames;

        /********************************/
        /* Sprawdzenie stanu klawiszy                       */
        /* (0 = puszczony, 1 = wciœniêty raz, 2 = trzymany) */

        for (a = 0; a < TETRIS_KEYS; a++)
        {
            if (GLFW_PRESS != glfwGetKey(window, KEYBOARD_MAP[a]))
            {
                Keys[a] = 0x00;
            }
            else
            {
                Keys[a]++;

                if (Keys[a] > 2)
                {
                    Keys[a] = 2;
                }
            }
        }

        /********************************/

        if (TETRIS_GAMESTATE_MENU == GameState)
        {
            if ((1 == Keys[TETRIS_KEY_LEFT]) || (1 == Keys[TETRIS_KEY_RIGHT]))
            {
                if (0 == GlobalPosition.x)
                {
                    if (0 == GlobalPosition.y)
                    {
                        /* Wejœcie do gry! */

                        Score = 0;

                        std::srand(std::time(NULL));
                        a = std::rand() % TETROMINOS;
                        b = 1 + std::rand() % 4;
                        NextBlock = std::rand() % TETROMINOS;
                        NextColor = 1 + std::rand() % 4;

                        setTetromino(a, b);

                        /* Left position: from [0] to [GRID_WIDTH - 1] */
                        /* Bottom position: falling from [GRID_HEIGT - 1] to [0] */
                        GlobalPosition.x = GRID_WIDTH / 2 - CurrentBlockSize / 2;
                        GlobalPosition.y = GRID_HEIGHT - 1;

                        std::memset(Grid, 0x00, sizeof(uint8_t) * GRID_SIZE);

                        GameState = TETRIS_GAMESTATE_PLAYING;

                        // (...) poprawiæ bezpoœrednie przejœcie z "glOrtho" na "gluPerspective"
                        setPerspectiveView(WindowWidth, WindowHeight);
                    }
                    else if (1 == GlobalPosition.y)
                    {
                        GlobalPosition.x = 1;
                        GlobalPosition.y = 0;
                    }
                    else
                    {
                        GameState = TETRIS_GAMESTATE_CLOSE;
                    }
                }
                else
                {
                    GlobalPosition.x = 0;
                    GlobalPosition.y = 0;
                }
            }
            else
            {
                if (1 == Keys[TETRIS_KEY_UP])
                {
                    GlobalPosition.y--;
                }
                else if (1 == Keys[TETRIS_KEY_DOWN])
                {
                    GlobalPosition.y++;
                }

                a = (0 == GlobalPosition.x) ? 2 : 0;

                if (GlobalPosition.y < 0)
                {
                    GlobalPosition.y = 0;
                }
                else if (GlobalPosition.y > a)
                {
                    GlobalPosition.y = a;
                }
            }
        }
        else if (TETRIS_GAMESTATE_PLAYING == GameState)
        {
            TotalFrames += DeltaFrames;
            collision = 0;

            if (Keys[TETRIS_KEY_PAUSE])
            {
                GlobalPosition.x = 0;
                GlobalPosition.y = 0;
                GameState = TETRIS_GAMESTATE_MENU;
            }
            else
            {
                if (checkCollision())
                {
                    GameState = TETRIS_GAMESTATE_TRYAGAIN;
                }

                if (1 == Keys[TETRIS_KEY_UP])
                {
                    a = GlobalPosition.x;

                    rotateTetrominoRight();

                    do
                    {
                        collision = checkCollision();

                        if (TETRIS_COLLISION_LEFTWALL == collision)
                        {
                            GlobalPosition.x++;
                        }
                        else if (TETRIS_COLLISION_RIGHTWALL == collision)
                        {
                            GlobalPosition.x--;
                        }
                        else if (TETRIS_COLLISION_OTHER == collision)
                        {
                            GlobalPosition.x = a;
                            rotateTetrominoLeft();
                            collision = 0;
                        }
                    }
                    while (0 != collision);
                }
                else if (1 == Keys[TETRIS_KEY_LEFT])
                {
                    GlobalPosition.x--;

                    if (checkCollision())
                    {
                        GlobalPosition.x++;
                    }
                }
                else if (1 == Keys[TETRIS_KEY_RIGHT])
                {
                    GlobalPosition.x++;

                    if (checkCollision())
                    {
                        GlobalPosition.x--;
                    }
                }

                if (Keys[TETRIS_KEY_DOWN])
                {
                    frames = STEP_FAST;
                }
                else
                {
                    frames = STEP_SLOW;
                }

                if (TotalFrames >= frames)
                {
                    TotalFrames -= frames;

                    GlobalPosition.y--;

                    if (TETRIS_COLLISION_OTHER == checkCollision())
                    {
                        GlobalPosition.y++;
                        collision = 1;
                    }
                }

                if (0 != collision)
                {
                    appendTetrominoToGrid();

                    setTetromino(NextBlock, NextColor);

                    NextBlock = std::rand() % TETROMINOS;
                    NextColor = 1 + std::rand() % 4;

                    GlobalPosition.x = GRID_WIDTH / 2 - CurrentBlockSize / 2;
                    GlobalPosition.y = GRID_HEIGHT - 1;

                    checkLines();
                }
            }
        }
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void setTetromino(uint8_t id, uint8_t color)
    {
        int32_t a;

        std::memset(CurrentBlock, 0x00, sizeof(uint8_t) * 4 * 4);

        CurrentBlockSize = TETROMINO_DIM[id];

        if (3 == CurrentBlockSize)
        {
            std::memcpy(CurrentBlock + 4, TETROMINO_SET[id] + 0, 3);
            std::memcpy(CurrentBlock + 8, TETROMINO_SET[id] + 3, 3);
        }
        else if (4 == CurrentBlockSize)
        {
            std::memcpy(CurrentBlock + 4, TETROMINO_SET[id], 2 * 4);
        }

        for (a = 0; a < 4 * 4; a++)
        {
            if (0 != CurrentBlock[a])
            {
                CurrentBlock[a] = color;
            }
        }
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void rotateTetrominoLeft()
    {
        uint8_t temp_blocks[4 * 4];

        if (3 == CurrentBlockSize)
        {
            temp_blocks[2 * 4 + 0] = CurrentBlock[0 * 4 + 0];
            temp_blocks[1 * 4 + 0] = CurrentBlock[0 * 4 + 1];
            temp_blocks[0 * 4 + 0] = CurrentBlock[0 * 4 + 2];
            temp_blocks[2 * 4 + 1] = CurrentBlock[1 * 4 + 0];
            temp_blocks[1 * 4 + 1] = CurrentBlock[1 * 4 + 1];
            temp_blocks[0 * 4 + 1] = CurrentBlock[1 * 4 + 2];
            temp_blocks[2 * 4 + 2] = CurrentBlock[2 * 4 + 0];
            temp_blocks[1 * 4 + 2] = CurrentBlock[2 * 4 + 1];
            temp_blocks[0 * 4 + 2] = CurrentBlock[2 * 4 + 2];
        }
        else if (4 == CurrentBlockSize)
        {
            temp_blocks[3 * 4 + 0] = CurrentBlock[0 * 4 + 0];
            temp_blocks[2 * 4 + 0] = CurrentBlock[0 * 4 + 1];
            temp_blocks[1 * 4 + 0] = CurrentBlock[0 * 4 + 2];
            temp_blocks[0 * 4 + 0] = CurrentBlock[0 * 4 + 3];
            temp_blocks[3 * 4 + 1] = CurrentBlock[1 * 4 + 0];
            temp_blocks[2 * 4 + 1] = CurrentBlock[1 * 4 + 1];
            temp_blocks[1 * 4 + 1] = CurrentBlock[1 * 4 + 2];
            temp_blocks[0 * 4 + 1] = CurrentBlock[1 * 4 + 3];
            temp_blocks[3 * 4 + 2] = CurrentBlock[2 * 4 + 0];
            temp_blocks[2 * 4 + 2] = CurrentBlock[2 * 4 + 1];
            temp_blocks[1 * 4 + 2] = CurrentBlock[2 * 4 + 2];
            temp_blocks[0 * 4 + 2] = CurrentBlock[2 * 4 + 3];
            temp_blocks[3 * 4 + 3] = CurrentBlock[3 * 4 + 0];
            temp_blocks[2 * 4 + 3] = CurrentBlock[3 * 4 + 1];
            temp_blocks[1 * 4 + 3] = CurrentBlock[3 * 4 + 2];
            temp_blocks[0 * 4 + 3] = CurrentBlock[3 * 4 + 3];
        }

        std::memcpy(CurrentBlock, temp_blocks, 4 * 4);
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void rotateTetrominoRight()
    {
        uint8_t temp_blocks[4 * 4];

        if (3 == CurrentBlockSize)
        {
            temp_blocks[0 * 4 + 2] = CurrentBlock[0 * 4 + 0];
            temp_blocks[1 * 4 + 2] = CurrentBlock[0 * 4 + 1];
            temp_blocks[2 * 4 + 2] = CurrentBlock[0 * 4 + 2];
            temp_blocks[0 * 4 + 1] = CurrentBlock[1 * 4 + 0];
            temp_blocks[1 * 4 + 1] = CurrentBlock[1 * 4 + 1];
            temp_blocks[2 * 4 + 1] = CurrentBlock[1 * 4 + 2];
            temp_blocks[0 * 4 + 0] = CurrentBlock[2 * 4 + 0];
            temp_blocks[1 * 4 + 0] = CurrentBlock[2 * 4 + 1];
            temp_blocks[2 * 4 + 0] = CurrentBlock[2 * 4 + 2];
        }
        else if (4 == CurrentBlockSize)
        {
            temp_blocks[0 * 4 + 3] = CurrentBlock[0 * 4 + 0];
            temp_blocks[1 * 4 + 3] = CurrentBlock[0 * 4 + 1];
            temp_blocks[2 * 4 + 3] = CurrentBlock[0 * 4 + 2];
            temp_blocks[3 * 4 + 3] = CurrentBlock[0 * 4 + 3];
            temp_blocks[0 * 4 + 2] = CurrentBlock[1 * 4 + 0];
            temp_blocks[1 * 4 + 2] = CurrentBlock[1 * 4 + 1];
            temp_blocks[2 * 4 + 2] = CurrentBlock[1 * 4 + 2];
            temp_blocks[3 * 4 + 2] = CurrentBlock[1 * 4 + 3];
            temp_blocks[0 * 4 + 1] = CurrentBlock[2 * 4 + 0];
            temp_blocks[1 * 4 + 1] = CurrentBlock[2 * 4 + 1];
            temp_blocks[2 * 4 + 1] = CurrentBlock[2 * 4 + 2];
            temp_blocks[3 * 4 + 1] = CurrentBlock[2 * 4 + 3];
            temp_blocks[0 * 4 + 0] = CurrentBlock[3 * 4 + 0];
            temp_blocks[1 * 4 + 0] = CurrentBlock[3 * 4 + 1];
            temp_blocks[2 * 4 + 0] = CurrentBlock[3 * 4 + 2];
            temp_blocks[3 * 4 + 0] = CurrentBlock[3 * 4 + 3];
        }

        std::memcpy(CurrentBlock, temp_blocks, 4 * 4);
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    // * 0 = pass
    // * 1 = collides with a left wall
    // * 2 = collides with a right wall
    // * 3 = collides with the bottom or the grid
    ////////////////////////////////////////////////////////////////
    int32_t checkCollision()
    {
        int32_t x, y, src = 0;

        int32_t x_dest = GlobalPosition.x;
        int32_t y_dest = GRID_HEIGHT - 1 - GlobalPosition.y - 3;

        for (y = 0; y < CurrentBlockSize; y++)
        {
            for (x = 0; x < CurrentBlockSize; x++)
            {
                if (0 != CurrentBlock[src])
                {
                    if (x_dest < 0)
                    {
                        return 1;
                    }
                    else if (x_dest >= GRID_WIDTH)
                    {
                        return 2;
                    }
                    else if (y_dest >= GRID_HEIGHT)
                    {
                        return 3;
                    }
                    else if ((y_dest >= 0) && (0 != Grid[y_dest * GRID_WIDTH + x_dest]))
                    {
                        return 3;
                    }
                }

                src++;
                x_dest++;
            }

            src = src - CurrentBlockSize + 4;
            x_dest = x_dest - CurrentBlockSize;

            y_dest++;
        }

        return 0;
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void appendTetrominoToGrid()
    {
        uint8_t color;
        int32_t x, y, src = 0;

        int32_t x_dest = GlobalPosition.x;
        int32_t y_dest = GRID_HEIGHT - 1 - GlobalPosition.y - 3;

        for (y = 0; y < CurrentBlockSize; y++)
        {
            for (x = 0; x < CurrentBlockSize; x++)
            {
                color = CurrentBlock[src];
                if ((0 != color) && (x_dest >= 0) && (x_dest < GRID_WIDTH) && (y_dest >= 0) && (y_dest < GRID_HEIGHT))
                {
                    Grid[y_dest * GRID_WIDTH + x_dest] = color;
                }

                src++;
                x_dest++;
            }

            src = src - CurrentBlockSize + 4;
            x_dest = x_dest - CurrentBlockSize;

            y_dest++;
        }
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void checkLines()
    {
        // (...)
    }

}
