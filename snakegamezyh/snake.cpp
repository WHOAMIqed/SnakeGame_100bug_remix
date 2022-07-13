#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake.h"


SnakeBody::SnakeBody()
{
}


SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

bool Snake::isPartOfSnake(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);
    for (int i = 0; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == temp)
        {
            return true;
        }
    }
    return false;
}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    if (headX <= 0 || headX >= this->mGameBoardWidth - 1)
    {
        return true;
    }
    if (headY <= 0 || headY >= this->mGameBoardHeight - 1)
    {
        return true;
    }
    return false;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf()
{
    SnakeBody& head = this->mSnake[0];
    // Exclude the snake head
    for (int i = 1; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == head)
        {
            return true;
        }
    }
    return false;
}

bool Snake::touchEnergyOrbit()
{
    SnakeBody head = this->mSnake[0];
    // Exclude the snake head
    int hx = head.getX();
    int hy = head.getY();
    for (int i = 0; i < this->mEnergyOrbit.size(); i ++)
    {
        int ex = mEnergyOrbit[i].getX();
        int ey = mEnergyOrbit[i].getY();
        if ((hx == ex)&&(hy == ey))
        {
            return true;
        }
    }
    return false;
}
/*bool Snake::touchEnergyOrbit()
{
    SnakeBody& head = this->mSnake[0];
    // Exclude the snake head
    for (int i = 0; i < this->mEnergyOrbit.size(); i ++)
    {
        if (this->mEnergyOrbit[i] == head)
        {
            return true;
        }
    }
    return false;
}*/
bool Snake::touchFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return true;
    }

        int x = mFood.getX();
        int y = mFood.getY();
        if (isPartOfSnake(x ,y)){
        return true;
        }
    else
    {
        return false;
    }
}
bool Snake::touchliveFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mlivefood == newHead)
    {
        return true;
    }

        int x = mlivefood.getX();
        int y = mlivefood.getY();
        if (isPartOfSnake(x ,y)){
        return true;
        }
    else
    {
        return false;
    }
}
void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}
void Snake::senselivefood(SnakeBody livefood)
{
    this->mlivefood = livefood;
}
// new
void Snake::senseEnergyOrbit(std::vector<SnakeBody> EnergyOrbit)
{
    this->mEnergyOrbit = EnergyOrbit;
}

std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

bool Snake::changeDirection(Direction newDirection)
{
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Down:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Left:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Right:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}


SnakeBody Snake::createNewHead()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    int headXNext;
    int headYNext;
    // new
    if (this->hitWall())
    {
        switch (this->mDirection)
    {
        case Direction::Up:
        {
            headXNext = headX;
            headYNext = mGameBoardHeight - 2;
            break;
        }
        case Direction::Down:
        {
            headXNext = headX;
            headYNext = 1;
            break;
        }
        case Direction::Left:
        {
            headXNext = mGameBoardWidth - 2;
            headYNext = headY;
            break;
        }
        case Direction::Right:
        {
            headXNext = 2;
            headYNext = headY;
            break;
        }
    }
    }
    else if (this->hitSelf()){
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            headXNext = headX;
            headYNext = headY - 2;
            break;
        }
        case Direction::Down:
        {
            headXNext = headX;
            headYNext = headY + 2;
            break;
        }
        case Direction::Left:
        {
            headXNext = headX - 2;
            headYNext = headY;
            break;
        }
        case Direction::Right:
        {
            headXNext = headX + 2;
            headYNext = headY;
            break;
        }
    }
    }
    else {
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            headXNext = headX;
            headYNext = headY - 1;
            break;
        }
        case Direction::Down:
        {
            headXNext = headX;
            headYNext = headY + 1;
            break;
        }
        case Direction::Left:
        {
            headXNext = headX - 1;
            headYNext = headY;
            break;
        }
        case Direction::Right:
        {
            headXNext = headX + 1;
            headYNext = headY;
            break;
        }
    }
    }

    SnakeBody newHead = SnakeBody(headXNext, headYNext);

    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
 //new
Terrain Snake::moveFoward()
{
    Terrain t;
    if (this->touchFood())
    {
        t = Terrain::Food;
        SnakeBody newHead = this->mFood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return t;
    }
    else if (this->touchEnergyOrbit())
    {
        t = Terrain::EnergyOrbit;
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return t;
    }
    else if (this->touchliveFood())
    {
        t = Terrain::livefood;
        SnakeBody newHead = this->mlivefood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return t;
    }
    else
    {
        t = Terrain::Plain;
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return t;
    }
}

/*bool Snake::moveFowardOrbit(){
    if ((!this->touchFood())&&this->touchEnergyOrbit())
    {
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return true;
    }
    else{
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return false;
    }
}*/

int Snake::getLength()
{
    return this->mSnake.size();
}

//zyh
//zyh
bool Snake::isPartOfFood(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);

        if (this->mFood == temp)
        {
            return true;
        }

    return false;
}

//zyh
bool Snake::hitObs()
{
    SnakeBody head = this->mSnake[0];
    //SnakeBody newHead = this->createNewHead();
    for (int i = 0; i < mObs.size();i++){
    if (this->mObs[i] == head)//newHead)
    {
        return true;
    }}

        return false;

}
bool Snake::dashhitObs()
{
    if(this ->judger_num == 1)
    {
    SnakeBody head = this->mSnake[0];
    SnakeBody headpre1 = this->createNewHead();
    //SnakeBody headpre2 = this->dashcreateNewHead();
    //SnakeBody newHead = this->createNewHead();
    for (int i = 0; i < mObs.size();i++){
    if (this->mObs[i] == head||this -> mObs[i] == headpre1)//newHead)
    {
        return true;
    }
    int x = mObs[i].getX();
    int y = mObs[i].getY();
    if (isPartOfSnake(x ,y)){
        return true;
    }
    }
    }
        return false;

}

void Snake::senseObs(std::vector<SnakeBody> obs)//zyh
{
    /*for (int i = 0; i < mObs.size();i++){
            obs.push_back(this->mObs[i]);
    }*/
    this -> mObs = obs;
}
SnakeBody Snake::dashcreateNewHead()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    int headXNext;
    int headYNext;

    switch (this->mDirection)
    {
        case Direction::Up:
        {
            headXNext = headX;
            headYNext = headY - 2;
            break;
        }
        case Direction::Down:
        {
            headXNext = headX;
            headYNext = headY + 2;
            break;
        }
        case Direction::Left:
        {
            headXNext = headX - 2;
            headYNext = headY;
            break;
        }
        case Direction::Right:
        {
            headXNext = headX + 2;
            headYNext = headY;
            break;
        }
    }

    SnakeBody newHead = SnakeBody(headXNext, headYNext);

    return newHead;
}
bool Snake::dash()
{
    if (this -> judger_num == 1)
    {
    /*if (this->touchFood())
    {
        SnakeBody newHead = this->mFood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return true;
    }*/
    /*if (this->dashtouchfood())
    {
        SnakeBody newHead = this->mFood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return true;
    }
    else

    {*/
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);

        int x = mFood.getX();
        int y = mFood.getY();
        if (isPartOfSnake(x ,y)){
        return true;
        }
        /*if (this->touchFood())
        {
            SnakeBody newHead = this->mFood;
            this->mSnake.insert(this->mSnake.begin(), newHead);
            return true;
        }
        if (this->dashtouchfood())
    {
        SnakeBody newHead = this->mFood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return true;
    }*/
        this->mSnake.pop_back();
        newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return false;
    }
    }

//zyh
bool Snake::dashtouchfood(){
    SnakeBody newHead = this->createNewHead();
    SnakeBody dashnewHead = this->dashcreateNewHead();
    SnakeBody head = this -> mSnake[0];
    if (this->mFood == newHead)
    {
        return true;
    }
    if (this->mFood == dashnewHead)
    {
        return true;
    }
    if (this->mFood == head)
    {
        return true;
    }
    else
    {
        return false;
    }

}
bool Snake::checkCollision()
{
    if (this->judger_num == 0){
    if (this->hitWall() || this->hitSelf() || this->hitObs()||this->dashhitObs())
    {
        return true;
    }
    else
    {
        return false;
    }}
    else{
        if (this->hitWall() || this->hitObs()||this->dashhitObs())
    {
        return true;
    }
    else
    {
        return false;
    }
    }
}
