#ifndef GAME_H
#define GAME_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>

#include "snake.h"

class Game
{
public:
    Game();
    ~Game();

		void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

		void createInstructionBoard();
    void renderInstructionBoard() const;

		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

		void renderBoards() const;

		void initializeGame();
    void runGame();
    void renderPoints() const;
    void renderDifficulty() const;
    // new
    void renderLives() const;
    void createRandomEnergyOrbit();
    void renderEnergyOrbit();

		void createRamdonFood();
    void renderFood() const;
    void renderSnake() const;
    void controlSnake() const;

		void startGame();
    bool renderRestartMenu() const;
    void adjustDelay();

        void play_BGM();
    //zyh
    void createRamdonObstacles();
    void renderObs() const;//zyh
private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 5;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    //new
    SnakeBody mEnergyOrbitCore;
    int mEnergyOrbitLength;
    const char mEnergyOrbitSymbol = '~';
    std::vector<SnakeBody> mEnergyOrbit;
    const char mFoodSymbol = '#';
    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    // new
    int mLives;
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    //zyh
    const char mObsSymbol = '%';
    std::vector<SnakeBody> mObs;
};

#endif
