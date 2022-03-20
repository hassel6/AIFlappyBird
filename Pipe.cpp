
#include "Pipe.h"

Pipe::Pipe( double x, double y, int id, Sprite sl, Sprite su ) {
    pipeX = x;
    pipeLowerY = y;
    pipeUpperY = pipeLowerY - 420;
    pipeID = id;

    pipeLowerSprite = sl;
    pipeUpperSprite = su;
}

void Pipe::funcDrawPipe( RenderWindow &w ) {
    pipeLowerSprite.setPosition( pipeX, pipeLowerY );
    pipeUpperSprite.setPosition( pipeX, pipeUpperY );
    w.draw( pipeLowerSprite );
    w.draw( pipeUpperSprite );
}

void Pipe::funcMovePipe( int gs ) {
    pipeX -= ( 1 * gs );
}
