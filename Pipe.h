#ifndef PIPE_H_INCLUDED
#define PIPE_H_INCLUDED

#include <SFML/Graphics.hpp>

using namespace sf;

class Pipe {

public:
    Sprite pipeLowerSprite, pipeUpperSprite;
    double pipeX = 0;
    double pipeLowerY = 0, pipeUpperY = 0;
    int pipeID = 0;

    Pipe( double x, double y, int id, Sprite sl, Sprite su );

    void funcDrawPipe( RenderWindow &w );
    void funcMovePipe( int gs );
};

#endif // PIPE_H_INCLUDED
