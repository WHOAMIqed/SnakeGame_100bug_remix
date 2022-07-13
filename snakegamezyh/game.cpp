#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

#include "game.h"



Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "'#' means food,'$' means livefood, and '%%% ' means obstracles!"); //new
    mvwprintw(this->mWindows[0], 3, 1, "You can press 'J' to dash!!!Be careful(O_o)If you hit a food with dash, you will choke to death!");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");
    mvwprintw(this->mWindows[2], 19, 1, "Lives");
    wrefresh(this->mWindows[2]);
}

//new
void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}
//new
void Game::renderLives() const
{
    std::string liveString = std::to_string(this->mLives);
    mvwprintw(this->mWindows[2], 20, 1, liveString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::createRandomEnergyOrbit()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i))//||this->mFood)
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mEnergyOrbitCore = availableGrids[random_idx];
}
void Game::renderEnergyOrbit()
{
    int snakeLength = this->mPtrSnake->getLength();
    int half = snakeLength;
    std::vector<SnakeBody> newEnergyOrbit;
    if ((this->mScreenHeight - this->mInformationHeight)/2 >= mEnergyOrbitCore.getY() && (this->mScreenWidth - this->mInstructionWidth)/2 >= mEnergyOrbitCore.getX())
        {
            for (int i=0;i<half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY(), this->mEnergyOrbitCore.getX()+i, this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX()+i,this->mEnergyOrbitCore.getY()));
            }
            for (int i=0;i<=half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY()+i, this->mEnergyOrbitCore.getX(), this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX(),this->mEnergyOrbitCore.getY()+i));
            }
        }
    else if ((this->mScreenHeight - this->mInformationHeight)/2 <= mEnergyOrbitCore.getY() && (this->mScreenWidth - this->mInstructionWidth)/2 >= mEnergyOrbitCore.getX())
        {
            for (int i=0;i<half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY(), this->mEnergyOrbitCore.getX()+i, this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX()+i,this->mEnergyOrbitCore.getY()));
            }
            for (int i=0;i<=half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY()-i, this->mEnergyOrbitCore.getX(), this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX(),this->mEnergyOrbitCore.getY()-i));
            }
        }
    else if ((this->mScreenHeight - this->mInformationHeight)/2 <= mEnergyOrbitCore.getY() && (this->mScreenWidth - this->mInstructionWidth)/2 <= mEnergyOrbitCore.getX())
        {
            for (int i=0;i<half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY(), this->mEnergyOrbitCore.getX()-i, this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX()-i,this->mEnergyOrbitCore.getY()));
            }
            for (int i=0;i<=half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY()-i, this->mEnergyOrbitCore.getX(), this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX(),this->mEnergyOrbitCore.getY()-i));
            }
        }
    else if ((this->mScreenHeight - this->mInformationHeight)/2 >= mEnergyOrbitCore.getY() && (this->mScreenWidth - this->mInstructionWidth)/2 <= mEnergyOrbitCore.getX())
        {
            for (int i=0;i<half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY(), this->mEnergyOrbitCore.getX()-i, this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX()-i,this->mEnergyOrbitCore.getY()));
            }
            for (int i=0;i<=half;i++)
            {
                mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY()+i, this->mEnergyOrbitCore.getX(), this->mEnergyOrbitSymbol);
                this->mEnergyOrbitLength++;
                newEnergyOrbit.push_back(SnakeBody( this->mEnergyOrbitCore.getX(),this->mEnergyOrbitCore.getY()+i));
            }
        }
    //mvwaddch(this->mWindows[1], this->mEnergyOrbitCore.getY(), this->mEnergyOrbitCore.getX(), this->mEnergyOrbitSymbol);
    mEnergyOrbit=newEnergyOrbit;
    wrefresh(this->mWindows[1]);
}
void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->createRamdonFood();

    //new
    this->createRandomEnergyOrbit();
    this->createRamdonObstacles();//zyh
    this->createRamdonlivefood();
    this->mPtrSnake->senseFood(this->mFood);
    this->mPtrSnake->senseObs(this->mObs);

    this->mPtrSnake->senseEnergyOrbit(this->mEnergyOrbit);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
    this->mPtrSnake->judger_num = 0;
    // new
    this->mLives = 5;
}

void Game::createRamdonFood()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
}

void Game::createRamdonlivefood()
{
    int appearlivefood = this->mPtrSnake->getLength();
    if (appearlivefood % 4 == 0)
    {
        std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mlivefood = availableGrids[random_idx];
    }
    else{
        mlivefood = SnakeBody(-100 , -100);
    }
}
//ÎÒÐ´µÄzyh
void Game::createRamdonObstacles(){
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i))
            {
                continue;
            }
            if(this->mPtrSnake->isPartOfFood(j, i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    //int random_idx = std::rand() % availableGrids.size();
    int snakeLength = this->mPtrSnake->getLength();
    int obsnum = snakeLength;
    mObs.clear();
    for (int j = 0; j < obsnum + 1;j++){
        mObs.push_back(SnakeBody(0,0));
    }
    for (int i = 0; i < mObs.size();i++){
            this->mObs[i] = availableGrids[std::rand() % availableGrids.size()];

}
}
void Game::renderObs() const
{
    for (int i = 0; i < mObs.size();i++){
    mvwaddch(this->mWindows[1], this->mObs[i].getY(), this->mObs[i].getX(), this->mObsSymbol);
    wrefresh(this->mWindows[1]);
    }
}
void Game::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
}
void Game::renderliveFood() const
{
    mvwaddch(this->mWindows[1], this->mlivefood.getY(), this->mlivefood.getX(), this->mlivefoodSymbol);
    wrefresh(this->mWindows[1]);
}
void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Game::controlSnake() const
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        case'J':
        case'j':
            {
                this->mPtrSnake->judger_num = 1;
                this->mPtrSnake->dash();
            }
        default:
        {
            break;
        }
    }
}

void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

void Game::runGame()
{
    bool moveSuccess;
    int key;
    //new
    int checkEnergyOrbit = 0;
    while (mLives>0)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);
        this->mPtrSnake->judger_num = 0;
        //new
        Terrain snackMove = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        //new

        /*if (snackMove==Terrain::EnergyOrbit)
        {
            checkEnergyOrbit =+ 1;
        }*/
        if (snackMove == Terrain::livefood){
            mLives++;
            this->mPoints += 1;
            this->createRamdonFood();
            this->createRamdonlivefood();
            //this->mEnergyOrbitLength=0;
            this->createRandomEnergyOrbit();
            this->createRamdonObstacles();
            this->mPtrSnake->senseFood(this->mFood);
            this->mPtrSnake->senseObs(this->mObs);
            this->mPtrSnake->senselivefood(this->mlivefood);
            this->adjustDelay();

            //mlivefood = SnakeBody(-100 , -100);

        }
        if (collision == true)
        {
            mLives--;
            //this->mPtrSnake->mInitialSnakeLength=mPoints+2;
            //this->mPtrSnake->initializeSnake();
            //continue; // new
        }
        this->renderSnake();
        if (snackMove == Terrain::Food)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->createRamdonlivefood();
            //this->mEnergyOrbitLength=0;
            this->createRandomEnergyOrbit();
            this->createRamdonObstacles();
            this->mPtrSnake->senseFood(this->mFood);
            this->mPtrSnake->senseObs(this->mObs);
            this->mPtrSnake->senselivefood(this->mlivefood);
            this->adjustDelay();
        }
        if (this->mPtrSnake->dash() == true)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            //for (int i = 0;i <= this->mPtrSnake->getLength();i++){
            this->mEnergyOrbitLength=0;
            this->createRandomEnergyOrbit();
            this->createRamdonObstacles();
            this->createRamdonlivefood();
            this->mPtrSnake->senseFood(this->mFood);
            this->mPtrSnake->senseObs(this->mObs);
             this->mPtrSnake->senselivefood(this->mlivefood);
            this->adjustDelay();
        }
        /*if (checkEnergyOrbit==this->mEnergyOrbitLength)
        {
            this->mPoints += 1;
            //this->createRamdonFood();

            this->mEnergyOrbitLength=0;
            this->createRandomEnergyOrbit();
            this->createRamdonObstacles();
            this->mPtrSnake->senseEnergyOrbit(this->mEnergyOrbit);
            this->adjustDelay();
            checkEnergyOrbit = 0;
        }*/
        this->renderFood();
        this->renderObs();//zyh
        this->renderliveFood();
        this->renderDifficulty();
        this->renderPoints();
        this->renderEnergyOrbit(); //new
        this->renderLives(); // new
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->mPtrSnake->judger_num = 0;//zyh
        refresh();
    }
}

void Game::startGame()
{
    refresh();
    bool choice;
    while (true)
    {
        this->readLeaderBoard();
        this->renderBoards();
        this->initializeGame();
        this->runGame();
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        choice = this->renderRestartMenu();
        if (choice == false)
        {
            break;
        }
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}











