#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

enum dir{no, left, right, up, down};

class Game
{
    public:
    Game(const char* title, int x, int y, int w, int h, Uint32 flags);
    void run();
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect snack;
    SDL_Rect square;
    bool isRunning;
    std::vector<SDL_Point> snake;
    dir direction;
    void update();
    void input();
    void render();
};