
#include "headers/Bird.h"

Bird::Bird( Sprite s, Matrix w1, Matrix w2 ) {
    birdSprite = s;
    b_weights1 = w1;
    b_weights2 = w2;

    birdBrain = NeuralNetwork( birdY, birdtoPipeX, birdToPipeY, b_weights1, b_weights2 );
}

void Bird::funcDrawBird( RenderWindow &w ) {
    birdSprite.setPosition( birdX, birdY );
    w.draw( birdSprite );
}

void Bird::funcUpdateBrain() {
    birdBrain.input1 = birdY;
    birdBrain.input2 = birdtoPipeX;
    birdBrain.input3 = birdToPipeY;
}

void Bird::funcBirdJump() {
    if( Bird::birdBrain.funcPredict()[ 0 ][ 0 ] < 0.5 ) {
        Bird::birdGravity = -2;
    }
}

void Bird::funcBirdLoop() {
    if( birdY < 0 ) {
        birdState = "dead";
    }
    else if( birdY < 380 ) {
        birdY += birdGravity;
        birdGravity += birdAccelertion;
        birdState = "alive";
    }
    else birdState = "dead";

    funcBirdJump();
}
