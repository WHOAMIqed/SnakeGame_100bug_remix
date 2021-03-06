#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};
//new
enum class Terrain
{
    Food = 0,
    EnergyOrbit = 1,
    Plain = 2,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    bool hitSelf();
    bool checkCollision();
    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    Terrain moveFoward();
    //new
    bool touchEnergyOrbit();
    void senseEnergyOrbit(std::vector<SnakeBody> EnergyOrbit);
    //bool moveFowardOrbit();
    //zyh
    bool dash();
    bool hitObs();
    void senseObs(std::vector<SnakeBody> obs);
    int judger_num = 0;
    bool dashtouchfood();
    SnakeBody dashcreateNewHead();
    bool dashhitObs();
    bool isPartOfFood(int x, int y);
private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    //new
    std::vector<SnakeBody> mEnergyOrbit;
    int mEnergyOrbitLength;

    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    std::vector<SnakeBody> mSnake;
    //zyh
    std::vector<SnakeBody> mObs;//zyh
};

#endif
