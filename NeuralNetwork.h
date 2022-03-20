#ifndef NEURALNETWORK_H_INCLUDED
#define NEURALNETWORK_H_INCLUDED

#include <vector>
#include <math.h>

using Matrix = std::vector<std::vector<double>>;

class NeuralNetwork {

public:

    double input1, input2, input3, input4;

    Matrix weights1 = Matrix( 3, std::vector<double>( 7 ) );
    Matrix weights2 = Matrix( 7, std::vector<double>( 1 ) );

    NeuralNetwork( double in1, double in2, double in3, Matrix w1, Matrix w2 );

    Matrix funcSigmoid( Matrix X );
    Matrix funcDot( Matrix wt1, Matrix wt2 );

    Matrix funcPredict();
};

#endif // NEURALNETWORK_H_INCLUDED
