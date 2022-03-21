
#include "headers/GameEngine.h"

#include <SFML/Graphics.hpp>

GameEngine::GameEngine( int population ){
    // WINDOW SETTINGS
    window.create( VideoMode( gameWindowWidth, gameWindowHigh ), "AI Flappy Birds!", Style::Default );
    window.setFramerateLimit( FPS );

    gamePopulationNumber = population;

    // LOAD AND SET TEXTURE AND SPRITES
    textureBackground.loadFromFile( "sprites/background.png" );
    textureBase.loadFromFile( "sprites/base.png" );
    texturePipeDown.loadFromFile( "sprites/pipedown.png" );
    texturePipeUp.loadFromFile( "sprites/pipeup.png" );
    textureBird.loadFromFile( "sprites/bird.png" );
    textureStart.loadFromFile( "sprites/message.png" );

    for( int i = 0; i < 10; i++ ) {
        std::string temp = "sprites/" + std::to_string( i ) + ".png";
        textureNumbers[ i ].loadFromFile( temp );
    }

    textureTexts[ 0 ].loadFromFile( "sprites/score.png" );
    textureTexts[ 1 ].loadFromFile( "sprites/alive.png" );
    textureTexts[ 2 ].loadFromFile( "sprites/generation.png" );
    textureTexts[ 3 ].loadFromFile( "sprites/gameover.png" );

    spriteBackground.setTexture( textureBackground );
    spriteBase.setTexture( textureBase );
    spritePipeDown.setTexture( texturePipeDown );
    spritePipeUp.setTexture( texturePipeUp );
    spriteBird.setTexture( textureBird );
    spriteStart.setTexture( textureStart );

    for( int i = 0; i < 10; i++ ) {
        spriteNumbers[ i ].setTexture( textureNumbers[ i ] );
    }

    spriteTexts[ 0 ].setTexture( textureTexts[ 0 ] );
    spriteTexts[ 1 ].setTexture( textureTexts[ 1 ] );
    spriteTexts[ 2 ].setTexture( textureTexts[ 2 ] );
    spriteTexts[ 3 ].setTexture( textureTexts[ 3 ] );

    // SET FIRST 4 PIPE AND PLACE TO WINDOW
    gamePipeList.push_back( Pipe( 300, funcRandomNumber( 220, 340 ), 0, spritePipeDown, spritePipeUp ) );
    gamePipeList.push_back( Pipe( 460, funcRandomNumber( 220, 340 ), 1, spritePipeDown, spritePipeUp ) );
    gamePipeList.push_back( Pipe( 620, funcRandomNumber( 220, 340 ), 2, spritePipeDown, spritePipeUp ) );
    gamePipeList.push_back( Pipe( 780, funcRandomNumber( 220, 340 ), 3, spritePipeDown, spritePipeUp ) );
    gamePipeID = 4;

    // WEIGHT SETTINGS ( INPUT LAYER OF 3 NEURONS, 1 HIDDEN LAYER OF 7 NEURONS, OUTPUT LAYER OF 1 NEURON )
    Matrix g_weights1 = Matrix( 3, std::vector<double>( 7 ) );
    Matrix g_weights2 = Matrix( 7, std::vector<double>( 1 ) );

    for( int i = 0; i < gamePopulationNumber; i++ ) {
        g_weights1 = funcGenerateWeights( 3, 7 );
        g_weights2 = funcGenerateWeights( 7, 1 );

        // GENERATE POPULATION
        gamePopulation.push_back( Bird( spriteBird, g_weights1, g_weights2 ) );
    }
}

// GENERATION OF RANDOM NUMBERS FOR CREATE RANDOM WEIGHTS
int GameEngine::funcRandomNumber( int mins, int maxs ) {
    int f = mins + ( rand() % ( maxs - mins ) );
    return f;
}

double GameEngine::funcRandomDouble( int mins, int maxs ) {
    double f = ( double )rand() / RAND_MAX;
    return mins + f * ( maxs - mins );
}

Matrix GameEngine::funcGenerateWeights( int row, int col ) {
    Matrix weights = Matrix( row, std::vector<double>( col ) );
    for( int i = 0; i < row; i++ ) {
        for( int j = 0; j < col; j++ ) {
            weights[ i ][ j ] = funcRandomDouble( -1, 1 );
        }
    }
    return weights;
}

void GameEngine::funcDrawTheGame() {

    window.clear();

    spriteBackground.setPosition( 0, 0 );
    spriteBase.setPosition( 0, 400 );

    window.draw( spriteBackground );
    window.draw( spriteBase );

    if( !gameStarted ) {
        spriteStart.setPosition( 51, 100 );
        window.draw( spriteStart );
    }
    else {
        for( Pipe elem : gamePipeList ) {
            elem.funcDrawPipe( window );
        }

        for( Bird elem : gamePopulation ) {
            elem.funcDrawBird( window );
        }
        printNumber( BOT, TEXT_SCORE, gamePopulation[ int( gamePopulation.size() ) - 1 ].birdScore );
        printNumber( TOP_RIGHT, TEXT_ALIVE, int( gamePopulation.size() ) );
        printNumber( TOP_LEFT, TEXT_GENERATION, gameGenerationTimer );
    }
    window.display();
}

void GameEngine::funcGeneCrossover() {
    sort( gameEliminatedBirds.begin(),
          gameEliminatedBirds.end(),
          [](const Bird& lhs, const Bird& rhs)
    {
        return lhs.birdScore < rhs.birdScore;
    });

    gameNextGeneration.clear();

    int best20Per = 10;//int( ( gamePopulationNumber - 1 ) * 0.98 );
    std::vector<Bird> bestBirds;

    for( int i = int( gameEliminatedBirds.size() )-1; i > int( gameEliminatedBirds.size() )-best20Per; i-- ) {
        gameEliminatedBirds[ i ].birdX = 50;
        gameEliminatedBirds[ i ].birdY = /*288/2*/50;
        gameEliminatedBirds[ i ].birdScore = 0;
        gameEliminatedBirds[ i ].birdGravity = 0;
        gameEliminatedBirds[ i ].birdState = "alive";
        gameEliminatedBirds[ i ].isCollide = false;
        gameNextGeneration.push_back( gameEliminatedBirds[ i ] );
    }
    gameEliminatedBirds.clear();

    for( int i = int( gameNextGeneration.size() ); i < gamePopulationNumber; i++ ) {
        Matrix rBird1_Weights1, rBird2_Weights1, chieldWeights1 = Matrix( 3, std::vector<double>( 7 ) );
        Matrix rBird1_Weights2, rBird2_Weights2, chieldWeights2 = Matrix( 7, std::vector<double>( 1 ) );

        Bird rBird1( spriteBird, rBird1_Weights1, rBird1_Weights2 );
        Bird rBird2( spriteBird, rBird2_Weights1, rBird2_Weights2 );

        int rn = funcRandomNumber( 0, int( gameNextGeneration.size() ) );

        rBird1 = gameNextGeneration[ rn ];
        rn = funcRandomNumber( 0, int( gameNextGeneration.size() ) );
        rBird2 = gameNextGeneration[ rn ];

        rBird1_Weights1 = rBird1.birdBrain.weights1;
        rBird1_Weights2 = rBird1.birdBrain.weights2;

        rBird2_Weights1 = rBird2.birdBrain.weights1;
        rBird2_Weights2 = rBird2.birdBrain.weights2;

        // Matrix ( 3, 7 )
        for( int i = 0; i < int( rBird1_Weights1.size() ); i++ ) {
            for( int j = 0; j < int( rBird1_Weights1[ i ].size() ); j++ ) {
                double rnd = funcRandomDouble( 0, 1 );
                if( rnd < 0.47 ) {
                    chieldWeights1[ i ][ j ] = rBird1_Weights1[ i ][ j ];
                }
                else if( rnd < 0.94 ) {
                    chieldWeights1[ i ][ j ] = rBird2_Weights1[ i ][ j ];
                }
                else { // Mutation Rate is %6
                    chieldWeights1[ i ][ j ] = funcRandomDouble( -1, 1 );
                }
            }
        }

        // Matrix ( 7, 1 )
        for( int i = 0; i < int( rBird1_Weights2.size() ); i++ ) {
            for( int j = 0; j < int( rBird1_Weights2[ i ].size() ); j++ ) {
                double rnd = funcRandomDouble( 0, 1 );
                if( rnd < 0.47 ) {
                    chieldWeights2[ i ][ j ] = rBird1_Weights2[ i ][ j ];
                }
                else if( rnd < 0.94 ) {
                    chieldWeights2[ i ][ j ] = rBird2_Weights2[ i ][ j ];
                }
                else { // Mutation Rate is %6
                    chieldWeights2[ i ][ j ] = funcRandomDouble( -1, 1 );
                }
            }
        }
        gameNextGeneration.push_back( Bird( spriteBird, chieldWeights1, chieldWeights2 ) );
    }
    gamePopulation.clear();
    gamePopulation = gameNextGeneration;
}

void GameEngine::funcRestartGame( ResType res ) {
    if( res == RESTART ) {
        gamePipeList.clear();
        gamePipeList.push_back( Pipe( 300, funcRandomNumber( 220, 340 ), 0, spritePipeDown, spritePipeUp ) );
        gamePipeList.push_back( Pipe( 460, funcRandomNumber( 220, 340 ), 1, spritePipeDown, spritePipeUp ) );
        gamePipeList.push_back( Pipe( 620, funcRandomNumber( 220, 340 ), 2, spritePipeDown, spritePipeUp ) );
        gamePipeList.push_back( Pipe( 780, funcRandomNumber( 220, 340 ), 3, spritePipeDown, spritePipeUp ) );
        gamePipeID = 4;
        gameGenerationTimer += 1;
        funcGeneCrossover();
    }
    if( res == FORCE_RESTART ) {
        for( Bird & elem : gamePopulation ) {
            gameEliminatedBirds.push_back( elem );
        }
        gamePopulation.clear();

        gamePipeList.clear();
        gamePipeList.push_back( Pipe( 300, funcRandomNumber( 220, 340 ), 0, spritePipeDown, spritePipeUp ) );
        gamePipeList.push_back( Pipe( 460, funcRandomNumber( 220, 340 ), 1, spritePipeDown, spritePipeUp ) );
        gamePipeList.push_back( Pipe( 620, funcRandomNumber( 220, 340 ), 2, spritePipeDown, spritePipeUp ) );
        gamePipeList.push_back( Pipe( 780, funcRandomNumber( 220, 340 ), 3, spritePipeDown, spritePipeUp ) );
        gamePipeID = 4;
        gameGenerationTimer += 1;
        funcGeneCrossover();
    }
}

bool GameEngine::isCollideLower( Pipe p, Bird b ) {

    double birdBounds[4][2] = {
        { b.birdX+2, b.birdY+2 }, { b.birdX + 32, b.birdY+2 },
        { b.birdX+2, b.birdY + 22 }, { b.birdX + 32, b.birdY + 22 }
    };
    double lowerBounds[4][2] = {
        { p.pipeX, p.pipeLowerY }, { p.pipeX + 52, p.pipeLowerY },
        { p.pipeX, p.pipeLowerY + 320 }, { p.pipeX + 52, p.pipeLowerY + 320 }
    };

    if( ( birdBounds[0][0] >= lowerBounds[3][0] || birdBounds[3][0] <= lowerBounds[0][0] ||
          birdBounds[0][1] >= lowerBounds[3][1] || birdBounds[3][1] <= lowerBounds[0][1] ) )
          return false;
    else
        return true;
}

bool GameEngine::isCollideUpper( Pipe p, Bird b ) {

    double birdBounds[4][2] = {
        { b.birdX+2, b.birdY+2 }, { b.birdX + 32, b.birdY+2 },
        { b.birdX+2, b.birdY + 22 }, { b.birdX + 32, b.birdY + 22 }
    };
    double upperBounds[4][2] = {
        { p.pipeX, p.pipeUpperY }, { p.pipeX + 52, p.pipeUpperY },
        { p.pipeX, p.pipeUpperY + 320 }, { p.pipeX + 52, p.pipeUpperY + 320 }
    };

    if( ( birdBounds[0][0] >= upperBounds[3][0] || birdBounds[3][0] <= upperBounds[0][0]
    || birdBounds[0][1] >= upperBounds[3][1] || birdBounds[3][1] <= upperBounds[0][1] ) )
        return false;
    else
        return true;
}

void GameEngine::gameLoop() {
    while( window.isOpen() ) {
        Event e;
        while( window.pollEvent( e ) ) {
            if( e.type == Event::Closed )
                window.close();
        }

        if( Keyboard::isKeyPressed( Keyboard::Enter ) ) {
            if( !gameStarted ) {
                gameStarted = true;
            }
        }

        if( !gameStarted ) {
            funcDrawTheGame();
            continue;
        }

        if( e.type == Event::KeyReleased ) {
            if( e.key.code == sf::Keyboard::Space )
                gameRestart = false;
        }

        if( Keyboard::isKeyPressed( Keyboard::Space ) ) {
            if( !gameRestart ) {
                gameRestart = true;
                funcRestartGame( FORCE_RESTART );
            }
        }

        std::cout << "Score : " << gamePopulation[ int( gamePopulation.size() ) - 1 ].birdScore
        << " || Generation : " << gameGenerationTimer << " || Alive Birds : "
        << int( gamePopulation.size() ) << std::endl;

        for( Pipe & elem : gamePipeList ) {
            if( elem.pipeX == -52 ) {
                elem.pipeX = 588;
                elem.pipeLowerY = funcRandomNumber( 220, 340 );
                elem.pipeUpperY = elem.pipeLowerY - 420;
                elem.pipeID = gamePipeID;
                gamePipeID++;
            }
            elem.funcMovePipe( gameSpeed );

            for( Bird & elem2 : gamePopulation ) {
                if( elem2.birdScore == elem.pipeID ) {
                    elem2.birdtoPipeX = elem.pipeX - elem2.birdX;
                    elem2.birdToPipeY = elem.pipeLowerY;
                    elem2.funcUpdateBrain();
                }

                bool collusion_l = isCollideLower( elem, elem2 );
                bool collusion_u = isCollideUpper( elem, elem2 );

                if( collusion_l || collusion_u ) {
                    gameEliminatedBirds.push_back( elem2 );
                    elem2.isCollide = true;
                }

                if( elem2.birdX + 16 == elem.pipeX ) {
                    elem2.birdScore++;
                }
            }
        }

        for( Bird & elem : gamePopulation ) {
            elem.funcBirdLoop();
            if( elem.birdState == "dead" ) {
                gameEliminatedBirds.push_back( elem );
                elem.isCollide = true;
            }
        }

        if( int( gamePopulation.size() ) == 0 ) {
            funcRestartGame( RESTART );
        }

        for( std::vector<Bird>::iterator it = gamePopulation.begin(); it != gamePopulation.end();) {
            if( it->isCollide == true ) {
                it = gamePopulation.erase( it );
            }
            else ++it;
        }

        funcDrawTheGame();
    }
}

void GameEngine::printNumber( PrintType type, TextType text, int num ) {

    int score = num;
    int digit = 0, y = 0, x = 0;

    while( score >= 1 ) {
        score = score / 10;
        digit++;
    }

    score = num;
    int* spScores = new int[ digit ];
    for( int i = 0; i < digit; i++ ) {
        spScores[ i ] = score%10;
        score = score/10;
    }

    switch( type ) {
        case TOP_LEFT: {
            x = 20; y = 40;
        }break;
        case TOP_RIGHT: {
            x = 240; y = 40;
        }break;
        case TOP: {
            x = 130; y = 40;
        }break;
        case BOT_LEFT: {
            x = 20; y = 440;
        }break;
        case BOT_RIGHT: {
            x = 240; y = 440;
        }break;
        case BOT: {
            x = 130; y = 440;
        }break;
    }

    switch( text ) {
        case TEXT_SCORE: {
            spriteTexts[ 0 ].setPosition( x-25, y-30 );
            window.draw( spriteTexts[ 0 ] );
        } break;
        case TEXT_ALIVE: {
            spriteTexts[ 1 ].setPosition( x-25, y-30 );
            window.draw( spriteTexts[ 1 ] );
        } break;
        case TEXT_GENERATION: {
            spriteTexts[ 2 ].setPosition( x-15, y-32 );
            window.draw( spriteTexts[ 2 ] );
        } break;
        case TEXT_GAMEOVER: {
            spriteTexts[ 3 ].setPosition( x-25, y-30 );
            window.draw( spriteTexts[ 3 ] );
        } break;
        case TEXT_NONE: {
        } break;
    }

    if( num == 0 ) {
        spriteNumbers[ 0 ].setPosition( x, y );
        window.draw( spriteNumbers[ 0 ] );
    }

    switch( digit ) {
        case 1: {
            spriteNumbers[ spScores[ 0 ] ].setPosition( x, y );
            window.draw( spriteNumbers[ spScores[ 0 ] ] );
        } break;
        case 2: {
            s1 = spriteNumbers[ spScores[ 1 ] ];
            s1.setPosition( x-13, y );
            spriteNumbers[ spScores[ 0 ] ].setPosition( x+13, y );
            window.draw( s1 );
            window.draw( spriteNumbers[ spScores[ 0 ] ] );
        } break;

        case 3: {
            s1 = spriteNumbers[ spScores[ 0 ] ];
            s1.setPosition( x+25, y );
            s2 = spriteNumbers[ spScores[ 1 ] ];
            s2.setPosition( x, y );
            spriteNumbers[ spScores[ 2 ] ].setPosition( x-25, y );
            spriteNumbers[ spScores[ 2 ] ].setPosition( x-25, y );
            window.draw( s1 );
            window.draw( s2 );
            window.draw( spriteNumbers[ spScores[ 2 ] ] );
        }  break;
    }
    delete[] spScores;
}
