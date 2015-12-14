#include <cstdlib>
#include <string.h>

#include "Game.hpp"

int main(int argc, char** argv) {

    Game    game;
    bool fullscreen=true;
    if(argc > 1){
        for(int i=1;i<argc;i++)
            if(strcmp(argv[i], "-w")==0 || strcmp(argv[i], "--window")==0)
                fullscreen=false;
    }

    game.init(fullscreen);
    game.run();

    return 0;
}
