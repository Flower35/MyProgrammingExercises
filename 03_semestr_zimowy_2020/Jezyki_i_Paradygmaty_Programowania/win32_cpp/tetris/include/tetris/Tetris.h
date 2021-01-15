#ifndef H_TETRIS
#define H_TETRIS

#define  _USE_MATH_DEFINES
#include <cmath>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <new>

#define  GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <Tetris/TetrisOther.h>

namespace Tetris
{

    ////////////////////////////////////////////////////////////////
    // Podstawowe definicje
    ////////////////////////////////////////////////////////////////

    #define GRID_WIDTH   10
    #define GRID_HEIGHT  24

    #define GAMEPLAY_WIDTH   256
    #define GAMEPLAY_HEIGHT  192

    #define SCREEN_WIDTH   320
    #define SCREEN_HEIGHT  240

    #define DEFAULT_WINDOW_WIDTH   640
    #define DEFAULT_WINDOW_HEIGHT  480

    #define WINDOW_NAME  "Tetris 2021"

    #define PERSPECTIVE_FOV       30.0  // 75.0
    #define PERSPECTIVE_Z_NEAR     1.0
    #define PERSPECTIVE_Z_FAR   5000.0

    #define CAMERA_OBSERVER_X      0.0
    #define CAMERA_OBSERVER_Y  -3250.0  // -1125.0
    #define CAMERA_OBSERVER_Z   +750.0

    #define CAMERA_TARGET_X  0.0
    #define CAMERA_TARGET_Y  0.0
    #define CAMERA_TARGET_Z  CAMERA_OBSERVER_Z


    ////////////////////////////////////////////////////////////////
    // Dodatkowe definicje
    ////////////////////////////////////////////////////////////////

    #define FRAMES_PER_SECOND  30
    #define STEP_SLOW          15
    #define STEP_FAST           3

    #define TETRIS_GAMESTATE_MENU       0
    #define TETRIS_GAMESTATE_PLAYING    1
    #define TETRIS_GAMESTATE_LINECLEAR  2
    #define TETRIS_GAMESTATE_TRYAGAIN   3
    #define TETRIS_GAMESTATE_CLOSE      4

    #define BLOCK_WIDTH  64

    #define TETRIS_KEY_UP     0
    #define TETRIS_KEY_LEFT   1
    #define TETRIS_KEY_RIGHT  2
    #define TETRIS_KEY_DOWN   3
    #define TETRIS_KEY_PAUSE  4
    #define TETRIS_KEYS       5

    #define TETRIS_COLLISION_LEFTWALL   1
    #define TETRIS_COLLISION_RIGHTWALL  2
    #define TETRIS_COLLISION_OTHER      3


    typedef struct
    {
        public:
            int32_t x;
            int32_t y;
    }
    POINT2I;

    typedef struct
    {
        public:
            double x;
            double y;
            double ratio;
    }
    WINDOWPARAMS;


    ////////////////////////////////////////////////////////////////
    // Zmienne globalne
    ////////////////////////////////////////////////////////////////

    extern WINDOWPARAMS WindowParams;

    extern int32_t WindowWidth;
    extern int32_t WindowHeight;

    extern uint8_t GameState;
    extern int32_t Score;
    extern uint8_t NextBlock;
    extern uint8_t NextColor;

    extern uint8_t * Grid;

    extern uint8_t CurrentBlockSize;
    extern uint8_t CurrentBlock[4 * 4];

    extern GLuint BlockTextureId;

    extern uint8_t Keys[TETRIS_KEYS];

    extern float PreviousFrames;
    extern float DeltaFrames;
    extern float TotalFrames;

    extern POINT2I GlobalPosition;


    ////////////////////////////////////////////////////////////////
    // Funkcje - rdzeñ gry
    ////////////////////////////////////////////////////////////////

    bool prepareTheGame();
    void endTheGame();


    ////////////////////////////////////////////////////////////////
    // Funkcje - logika gry
    ////////////////////////////////////////////////////////////////

    void doTheLogic(GLFWwindow * window);

    void setTetromino(uint8_t id, uint8_t color);
    void rotateTetrominoLeft();
    void rotateTetrominoRight();

    int32_t checkCollision();
    void appendTetrominoToGrid();
    void checkLines();


    ////////////////////////////////////////////////////////////////
    // Funkcje - wizualizacja gry
    ////////////////////////////////////////////////////////////////

    void doTheDrawing(GLFWwindow * window);

    void drawABorder();
    void drawABlock(float x, float z, uint8_t color);
    void drawAGrid(const uint8_t * grid, int32_t width, int32_t height);

    void setOrthoView(int width, int height);
    void setPerspectiveView(int width, int height);
    void windowResizeCallback(GLFWwindow * window, int width, int height);

}

#endif
