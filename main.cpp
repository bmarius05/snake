
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <windows.h>

#define INC 10

struct snakeCube{
    SDL_FRect rectangleCoordinates;
    SDL_Color rectangleColor;
    char dir;
};

std::vector<snakeCube> snake;

//define window dimensions
constexpr int WINDOW_WIDTH { 1000 };
constexpr int WINDOW_HEIGHT { 700 };

float scale=50;
float appleSize=10;

bool closedSides=false;

SDL_Window* window { nullptr };
SDL_Renderer* renderer { nullptr };
SDL_Event currentEvent;

SDL_Color backgroundColor { 255, 255, 255, 255 };
SDL_Color snakeColor {255,0,0,255};
SDL_Color headColor {200,0,0,255};

float startX=-1;
float startY=-1;

int lastColor=0;

int dir = 1;

bool quit { false };

float mouseX { -1.0f }, mouseY { -1.0f };

float displayScale { 1.0f };

Uint32 lastCall = SDL_GetTicks();
Uint32 interval = 400;

snakeCube apple={{-100,-100,appleSize,appleSize},{0,240,0,255},-1};

float speed=scale;
int dx[]={0,1,0,-1};
int dy[]={-1,0,1,0};


bool initWindow()
{
    bool success { true };
    
    //Try to initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL initialization failed: %s\n", SDL_GetError());
        success = false;
    }
    else{
        //Try to create the window and renderer
        displayScale = SDL_GetDisplayContentScale(1);        
        
        if(!SDL_CreateWindowAndRenderer(
                "Snake Game", 
                static_cast<int>(displayScale * WINDOW_WIDTH), 
                static_cast<int>(displayScale * WINDOW_HEIGHT), 
                SDL_WINDOW_HIGH_PIXEL_DENSITY, 
                &window, &renderer))
        {
            SDL_Log("Failed to create window and renderer: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Apply global display scaling to renderer
            SDL_SetRenderScale(renderer, displayScale, displayScale);

            //Set background color
            SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

            //Apply background color
            SDL_RenderClear(renderer);
        }
    }
    
    return success;
}

void gameOver(){
    printf("\n-----GAME OVER-----\n");
    Sleep(2500);
    quit=true;
}

void destroyWindow()
{
    //Destroy renderer
    if (renderer)
        SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    //Destroy window
    if (window)
        SDL_DestroyWindow(window);
    window = nullptr;
    
    //Quit SDL
    SDL_Quit();
}

void addPiece(){
    snakeCube last = snake.back();
    float posx = last.rectangleCoordinates.x+dx[last.dir]*-1*scale;
    float posy = last.rectangleCoordinates.y+dy[last.dir]*-1*scale;
    snakeCube piece = {{posx,posy,scale,scale},snakeColor,last.dir};
    snake.push_back(piece);
}

void generateApple(){
    bool ok;
    int x,y,alignment;
    do{
        ok=true;
        x = rand()%(WINDOW_WIDTH-(int)scale);
        y = rand()%(WINDOW_HEIGHT-(int)scale);
        alignment=scale;
        x=(x-x%alignment)+(alignment-appleSize)/2;
        y=(y-y%alignment)+(alignment-appleSize)/2;
        for(snakeCube s: snake){
            if(x>=s.rectangleCoordinates.x&&x<=s.rectangleCoordinates.x+scale&&y>=s.rectangleCoordinates.y&&y<=s.rectangleCoordinates.y+scale){
                ok=false;
                break;
            }
        }
    }while(!ok);
    apple = {{(float)x,(float)y,appleSize,appleSize},{0,240,0,255},-1};
}
void moveSnake(){
    snake[0].dir=dir;
    for(int i=0;i<snake.size();i++){
        for(int j=0;j<i;j++){
            if(snake[i].rectangleCoordinates.x+speed*dx[snake[i].dir]==snake[j].rectangleCoordinates.x&&snake[i].rectangleCoordinates.y+speed*dy[snake[i].dir]==snake[j].rectangleCoordinates.y)
                gameOver();
        }
        snake[i].rectangleCoordinates.x+=speed*dx[snake[i].dir];
        if(snake[i].rectangleCoordinates.x>=WINDOW_WIDTH){
            if(closedSides&&i==0)
                gameOver();
            snake[i].rectangleCoordinates.x-=WINDOW_WIDTH;
        }
        if(snake[i].rectangleCoordinates.x<0){
            if(closedSides&&i==0)
                gameOver();
            snake[i].rectangleCoordinates.x+=WINDOW_WIDTH;
        }
        snake[i].rectangleCoordinates.y+=speed*dy[snake[i].dir];
        if(snake[i].rectangleCoordinates.y>=WINDOW_HEIGHT){
            if(closedSides&&i==0)
                gameOver();
            snake[i].rectangleCoordinates.y-=WINDOW_HEIGHT;
        }
        if(snake[i].rectangleCoordinates.y<0){
            if(closedSides&&i==0)
                gameOver();
            snake[i].rectangleCoordinates.y+=WINDOW_HEIGHT;
        }
    }
    for(int i=snake.size()-1;i>0;i--)
        snake[i].dir=snake[i-1].dir;
}
void timedFunc(){
    moveSnake();
    float headx=snake[0].rectangleCoordinates.x;
    float heady=snake[0].rectangleCoordinates.y;
    float applex=apple.rectangleCoordinates.x;
    float appley=apple.rectangleCoordinates.y;
    printf("x = %.2f, y = %.2f\n",applex,appley);
    if(applex>=headx&&applex<=headx+scale&&appley>=heady&&appley<=heady+scale){
        generateApple();
        addPiece();
    }
}


void renderAll(){
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, apple.rectangleColor.r, apple.rectangleColor.g, apple.rectangleColor.b, apple.rectangleColor.a);
    SDL_RenderFillRect(renderer, &apple.rectangleCoordinates);
    for(snakeCube s: snake){
        SDL_SetRenderDrawColor(renderer, s.rectangleColor.r, s.rectangleColor.g, s.rectangleColor.b, s.rectangleColor.a);
        SDL_RenderFillRect(renderer, &s.rectangleCoordinates);
    }
    
    SDL_RenderPresent(renderer);  
}

int main(int argc, char * argv[]) {
    srand(time(0));
    if(!initWindow())
    {
        SDL_Log("Failed to initialize");
        return -1;
    }
    
    SDL_zero(currentEvent);

    snakeCube head = {{0,0,scale,scale},headColor,1};
    snake.push_back(head);
    generateApple();
    
    while (!quit) {
        Uint32 now = SDL_GetTicks();
        if(now-lastCall>=interval){
            timedFunc();
            lastCall=now;
            renderAll();  
        }
        while(SDL_PollEvent(&currentEvent))
        {
            //User requests quit
            if(currentEvent.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            
            //Keyboard event
            if(currentEvent.type == SDL_EVENT_KEY_DOWN)
            {
                switch(currentEvent.key.key)
                {
                    case SDLK_UP:
                        if(snake[0].dir!=2)
                            dir=0;
                        break;
                    case SDLK_RIGHT:
                        if(snake[0].dir!=3)
                            dir=1;
                        break;
                    case SDLK_DOWN:
                        if(snake[0].dir!=0)
                            dir=2;
                        break;
                    case SDLK_LEFT:
                        if(snake[0].dir!=1)
                            dir=3;
                        break;
                    case SDLK_Q:
                        gameOver();
                        break;
                    default:
                        break;
                }
            }    
            renderAll();            
        }
    }
    
    destroyWindow();
    return 0;
}
