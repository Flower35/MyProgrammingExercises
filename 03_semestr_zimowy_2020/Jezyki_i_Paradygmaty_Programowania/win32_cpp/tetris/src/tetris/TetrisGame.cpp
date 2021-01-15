////////////////////////////////////////////////////////////////
// "TetrisGame.cpp"
////////////////////////////////////////////////////////////////

#include <Tetris/Tetris.h>


namespace Tetris
{

    ////////////////////////////////////////////////////////////////
    // Zmienne globalne
    ////////////////////////////////////////////////////////////////

    WINDOWPARAMS WindowParams;

    int32_t WindowWidth;
    int32_t WindowHeight;

    uint8_t GameState;
    int32_t Score;
    uint8_t NextBlock;
    uint8_t NextColor;

    uint8_t * Grid;

    uint8_t CurrentBlockSize;
    uint8_t CurrentBlock[4 * 4];

    GLuint BlockTextureId;

    uint8_t Keys[TETRIS_KEYS];

    float PreviousFrames;
    float DeltaFrames;
    float TotalFrames;

    POINT2I GlobalPosition;


    ////////////////////////////////////////////////////////////////
    // Lista definicji Tetromino
    ////////////////////////////////////////////////////////////////

    const uint8_t * TETROMINO_SET[TETROMINOS] =
    {
        TETROMINO_T,
        TETROMINO_J,
        TETROMINO_Z,
        TETROMINO_O,
        TETROMINO_S,
        TETROMINO_L,
        TETROMINO_I
    };


    ////////////////////////////////////////////////////////////////
    // Funkcja przygotowuj¹ca grê
    ////////////////////////////////////////////////////////////////
    bool prepareTheGame()
    {
        size_t x, y, a, b;
        uint8_t dummy_color;
        uint8_t * dummy_pixels = nullptr;

        const size_t TEXTURE_SIZE = BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_WIDTH;
        const size_t GRID_SIZE = GRID_WIDTH * GRID_HEIGHT;

        /********************************/

        WindowParams.x     = 0.5 * SCREEN_WIDTH;
        WindowParams.y     = 0.5 * SCREEN_HEIGHT;
        WindowParams.ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

        WindowWidth  = DEFAULT_WINDOW_WIDTH;
        WindowHeight = DEFAULT_WINDOW_HEIGHT;

        GameState = TETRIS_GAMESTATE_MENU;
        Grid = nullptr;
        BlockTextureId = 0;

        std::memset(Keys, 0x00, sizeof(uint8_t) * TETRIS_KEYS);

        PreviousFrames = 0;
        DeltaFrames = 0;
        TotalFrames = 0;

        GlobalPosition.x = 0;
        GlobalPosition.y = 0;

        /********************************/

        glClearColor(0.125f, 0.125f, 0.125f, 1.0f);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_TEXTURE_2D);

        /********************************/

        glGenTextures(1, &BlockTextureId);

        glBindTexture(GL_TEXTURE_2D, BlockTextureId);

        dummy_pixels = new (std::nothrow) uint8_t [3 * TEXTURE_SIZE];
        if (nullptr == dummy_pixels)
        {
            return false;
        }

        a = 0;
        for (y = 0; y < BLOCK_TEXTURE_WIDTH; y++)
        {
            for (x = 0; x < BLOCK_TEXTURE_WIDTH; x++)
            {
                dummy_color = 0xFF;

                for (b = 0; b < 4; b++)
                {
                    if
                    (
                      (b == x) || ((BLOCK_TEXTURE_WIDTH - b - 1) == x)
                      || (b == y) || ((BLOCK_TEXTURE_WIDTH - b - 1) == y)
                    )
                    {
                        b = 3;
                    }
                    else
                    {
                        dummy_color += 0x40;
                    }
                }

                for (b = 0; b < 3; b++)
                {
                    dummy_pixels[a] = dummy_color;
                    a++;
                }
            }
        }

        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGB8,
            BLOCK_TEXTURE_WIDTH,
            BLOCK_TEXTURE_WIDTH,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            dummy_pixels
        );

        delete[](dummy_pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);

        /********************************/

        Grid = new (std::nothrow) uint8_t [GRID_SIZE];
        if (nullptr == Grid)
        {
            return false;
        }

        /********************************/

        return true;
    }


    ////////////////////////////////////////////////////////////////
    // (...)
    ////////////////////////////////////////////////////////////////
    void endTheGame()
    {
        if (0 != BlockTextureId)
        {
            glDeleteTextures(1, &BlockTextureId);
        }

        if (nullptr != Grid)
        {
            delete[](Grid);
        }
    }

}
