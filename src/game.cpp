#include <game.h>
#define INPUT_REFRESH 30
#define RENDER_REFRESH 10
#define WIDTH 720
#define HEIGHT 720
#define ROWS 20
#define COLUMNS 20
#define COLUMN_WIDTH WIDTH/COLUMNS
#define ROW_HEIGHT HEIGHT/ROWS

Game::Game(const char* title, int x, int y, Uint32 flags)
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) std::cout << "ERROR: COULDN'T INIT SDL2\n";
    window = SDL_CreateWindow(title, x, y, WIDTH, HEIGHT, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    direction = dir::no;
    square.w = snack.w = COLUMN_WIDTH;
    square.h = snack.h = ROW_HEIGHT;
    snake.push_back({0, 0});
    srand((unsigned)time(NULL));
    snack.x = COLUMN_WIDTH*(rand()%(COLUMNS-1)+1); snack.y = ROW_HEIGHT*(rand()%(ROWS-1)+1);
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
        acc++; if(acc==INPUT_REFRESH/RENDER_REFRESH)
        {
        update();
        render();
        acc = 0;
        }
        deltaTime = SDL_GetTicks() - timeValue;
        if(deltaTime<1000/INPUT_REFRESH) SDL_Delay(1000/INPUT_REFRESH-deltaTime);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Your score is: " << snake.size()-1 << "\n";
}

void Game::update() 
{
    if(snake[0].x<0||snake[0].x>=WIDTH||snake[0].y<0||snake[0].y>=HEIGHT) isRunning = false;
    else if(snake[0].x==snack.x&&snake[0].y==snack.y)
    {
        snake.push_back({0, 0});
        snack.x = COLUMN_WIDTH*(rand()%(COLUMNS-1)+1); snack.y = ROW_HEIGHT*(rand()%(ROWS-1)+1);
        for(int i=0;i<snake.size();i++)
        {
            if(snake[i].x==snack.x&&snake[i].y==snack.y)
            {
                snack.x = COLUMN_WIDTH*(rand()%(COLUMNS-1)+1); snack.y = ROW_HEIGHT*(rand()%(ROWS-1)+1);
                i = 0;
            }
        }
    }
    for(int i = snake.size(); i>0 ; i--)
    {
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }
    switch(direction)
    {
        case no: break;
        case left: snake[0].x-=COLUMN_WIDTH; break;
        case right: snake[0].x+=COLUMN_WIDTH; break;
        case up: snake[0].y-=ROW_HEIGHT; break;
        case down: snake[0].y+=ROW_HEIGHT; break;
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