#include <game.h>

int main(int argc, char* argv[])
{
    Game game("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SDL_WINDOW_SHOWN);
    game.run();
    return 0;
}