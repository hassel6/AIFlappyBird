
#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork( double in1, double in2, double in3, Matrix w1, Matrix w2 ){
    input1 = in1;
    input2 = in2;
    input3 = in3;

    weights1 = w1;
    weights2 = w2;
}

Matrix NeuralNetwork::funcSigmoid( Matrix X ) {
    for( int i = 0; i < int( X.size() ); i++ ) {
        for( int j = 0; j < int( X[ i ].size() ); j++ ) {
            X[ i ][ j ] = 1 / ( 1 + std::exp( -1*X[ i ][ j ] ) );
        }
    }
    return X;
}

Matrix NeuralNetwork::funcDot( Matrix wt1, Matrix wt2 ) {

    int r1, c1, c2;
    r1 = int( wt1.size() ); c1 = int( wt1[ 0 ].size() ); c2 = int( wt2[ 0 ].size() );
    Matrix outputWeights = Matrix( r1, std::vector<double>( c2 ) );

    for( int i = 0; i < r1; i++ ) {
        for( int j = 0; j < c2; j++ ) {
            for( int k = 0; k < c1; k++ ) {
                outputWeights[ i ][ j ] += wt1[ i ][ k ] * wt2[ k ][ j ];
            }
        }
    }
    return outputWeights;
}

Matrix NeuralNetwork::funcPredict() {
    Matrix inputLayer = Matrix( 1, std::vector<double>( 3 ) );
    inputLayer[ 0 ][ 0 ] = input1;
    inputLayer[ 0 ][ 1 ] = input2;
    inputLayer[ 0 ][ 2 ] = input3;

    Matrix hiddenLayer1 = funcDot( inputLayer, weights1 );
    hiddenLayer1 = funcSigmoid( hiddenLayer1 );

    Matrix hiddenLayer2 = funcDot( hiddenLayer1, weights2 );
    hiddenLayer2 = funcSigmoid( hiddenLayer2 );
    // returns 1x1 matrix
    return hiddenLayer2;
}
