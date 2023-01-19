#include <game.h>
#define FPS 30

Game::Game(const char* title, int x, int y, int w, int h, Uint32 flags)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) std::cout << "ERROR: COULDN'T INIT SDL2\n";
    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    direction = dir::no;
    square.w = square.h = snack.w = snack.h = 25;
    snake.push_back({0, 0});
    srand((unsigned)time(NULL));
    snack.x = 25*(rand()%19+1); snack.y = 25*(rand()%19+1);
}

void Game::run()
{
    isRunning = true;
    unsigned int timeValue;
    unsigned int deltaTime;
    int acc = 0;
    while(isRunning)
    {
        timeValue = SDL_GetTicks();
        input();
        acc++; if(acc==3)
        {
        update();
        render();
        acc = 0;
        }
        deltaTime = SDL_GetTicks() - timeValue;
        if(deltaTime<1000/FPS) SDL_Delay(1000/FPS-deltaTime);
    }
    std::cout << "Your score is: " << snake.size()-1 << "\n";
    SDL_DestroyWindow(window);
}

void Game::update() 
{
    if(snake[0].x<0||snake[0].x>=500||snake[0].y<0||snake[0].y>=500) isRunning = false;
    else if(snake[0].x==snack.x&&snake[0].y==snack.y)
    {
        snake.push_back({0, 0});
        snack.x = 25*(rand()%19+1); snack.y = 25*(rand()%19+1);
    }
    for(int i = snake.size(); i>0 ; i--)
    {
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }
    switch(direction)
    {
        case no: break;
        case left: snake[0].x-=25; break;
        case right: snake[0].x+=25; break;
        case up: snake[0].y-=25; break;
        case down: snake[0].y+=25; break;
        default: std::cout << "ERROR: ENUM ERROR 1\n";
    }
    for(int i = 1; i<snake.size(); i++) if(snake[0].x==snake[i].x&&snake[0].y==snake[i].y) isRunning = false;
}

void Game::input()
{
    SDL_Event event; SDL_PollEvent(&event); if(event.type==SDL_QUIT) isRunning = false;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if(keystates[SDL_SCANCODE_ESCAPE]) isRunning = false;
    if(keystates[SDL_SCANCODE_LEFT]&&direction!=dir::right) direction = dir::left;
    if(keystates[SDL_SCANCODE_RIGHT]&&direction!=dir::left) direction = dir::right;
    if(keystates[SDL_SCANCODE_UP]&&direction!=dir::down) direction = dir::up;
    if(keystates[SDL_SCANCODE_DOWN]&&direction!=dir::up) direction = dir::down;
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for(int i = 0; i < snake.size(); i++)
    {
        square.x = snake[i].x; square.y = snake[i].y;
        SDL_RenderFillRect(renderer ,&square);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &snack);
    SDL_RenderPresent(renderer);
}