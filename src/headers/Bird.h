#ifndef BIRD_H_INCLUDED
#define BIRD_H_INCLUDED

#include "NeuralNetwork.h"

#include <SFML/Graphics.hpp>

using namespace sf;
using Matrix = std::vector<std::vector<double>>;

class Bird {

public:
    Sprite birdSprite;

    float birdAccelertion = 0.075;

    Matrix b_weights1 = Matrix( 3, std::vector<double>( 7 ) );
    Matrix b_weights2 = Matrix( 7, std::vector<double>( 1 ) );

    double birdX = 50, birdY = /*288/2*/50;
    double birdtoPipeX = 0, birdToPipeY = 0;
    float birdGravity = 0.75;

    int birdScore = 0;
    bool isCollide = false;
    std::string birdState = "alive";

    Bird( Sprite s, Matrix w1, Matrix w2 );

    NeuralNetwork birdBrain = NeuralNetwork( birdY, birdtoPipeX, birdToPipeY, b_weights1, b_weights2 );

    void funcDrawBird( RenderWindow &w );
    void funcUpdateBrain();
    void funcBirdJump();
    void funcBirdLoop();
};


#endif // BIRD_H_INCLUDED
