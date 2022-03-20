#ifndef GAMEENGINE_H_INCLUDED
#define GAMEENGINE_H_INCLUDED

#include "Bird.h"
#include "Pipe.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <random>
#include <string>

using namespace sf;

class GameEngine {
private:
    const unsigned int FPS = 60;

    int gameWindowHigh = 512, gameWindowWidth = 288;
    int gameSpeed = 2;
    int gamePipeCount = 0, gamePipeID = 4;
    int gamePopulationNumber = 0, gameGenerationTimer = 1;

    std::vector<Bird> gamePopulation, gameNextGeneration, gameEliminatedBirds;
    std::vector<Pipe> gamePipeList;

    Texture textureBackground, textureBase, texturePipeUp, texturePipeDown, textureBird, textureStart;
    Sprite spriteBackground, spriteBase, spritePipeUp, spritePipeDown, spriteBird, spriteStart, s1, s2;

    std::vector<Texture> textureNumbers = std::vector<Texture>( 10 );
    std::vector<Sprite> spriteNumbers = std::vector<Sprite>( 10 );

    std::vector<Texture> textureTexts = std::vector<Texture>( 4 );
    std::vector<Sprite> spriteTexts = std::vector<Sprite>( 4 );

    bool gameStarted = false;
    bool gameRestart = false;
    bool gameSaveWeights = false;

public:
    GameEngine( int population );

    RenderWindow window;

    int funcRandomNumber( int mins, int maxs );
    double funcRandomDouble( int mins, int maxs );

    Matrix funcGenerateWeights( int row, int col );

    void funcDrawTheGame();
    void funcGeneCrossover();

    enum ResType { RESTART, FORCE_RESTART };
    void funcRestartGame( ResType res );

    void gameLoop();

    enum PrintType { TOP, TOP_LEFT, TOP_RIGHT, BOT, BOT_LEFT, BOT_RIGHT };
    enum TextType { TEXT_SCORE, TEXT_GENERATION, TEXT_ALIVE, TEXT_GAMEOVER, TEXT_NONE };
    void printNumber( PrintType type, TextType, int num );

    bool isCollideLower( Pipe p, Bird b );
    bool isCollideUpper( Pipe p, Bird b );
};

#endif // GAMEENGINE_H_INCLUDED
