#include <stdio.h>
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>

int WIDTH = 400, HEIGHT = 500;
int gameWidth = 9, gameHeight = 9;

struct mine_sites{
    SDL_Rect quadrant;
    bool hasmine = false;
    bool isshown = false;
    bool hasflag = false;
    int minesnear = 0;
};
typedef struct mine_sites blocks;

struct grid_textures{
    SDL_Texture *one;
    SDL_Texture *two;
    SDL_Texture *three;
    SDL_Texture *four;
    SDL_Texture *five;
    SDL_Texture *six;
    SDL_Texture *seven;
    SDL_Texture *eight;

    SDL_Texture *empty;
    SDL_Texture *block;
    SDL_Texture *flag;
    SDL_Texture *mine;
    SDL_Texture *blown_mine;

    SDL_Texture *frame_easy;

    SDL_Texture *smiley_neutral;
};

struct counter_textures{
    SDL_Texture *zero;
    SDL_Texture *one;
    SDL_Texture *two;
    SDL_Texture *three;
    SDL_Texture *four;
    SDL_Texture *five;
    SDL_Texture *six;
    SDL_Texture *seven;
    SDL_Texture *eight;
    SDL_Texture *nine;
};
struct all_textures{
    SDL_Texture *frame_easy;
    SDL_Texture *smiley_neutral;
    struct grid_textures grid;
    struct counter_textures counter;
};
typedef all_textures gameTxtr;

struct mine_position{
    int x;
    int y;
};
typedef struct mine_position minePos;

struct mine_counter_display{
    SDL_Rect frame;
    SDL_Rect slot1;
    SDL_Rect slot2;
    SDL_Rect slot3;
};
typedef mine_counter_display mcDisp;

struct timer_display{
    SDL_Rect frame;
    SDL_Rect slot1;
    SDL_Rect slot2;
    SDL_Rect slot3;
};
typedef timer_display tmrDisp;



void init_textures(gameTxtr *textures, SDL_Renderer *renderer){
    SDL_Surface *surface = SDL_LoadBMP("./textures/grid/one.bmp");
    textures->grid.one = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/two.bmp");
    textures->grid.two = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/three.bmp");
    textures->grid.three = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/four.bmp");
    textures->grid.four = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/five.bmp");
    textures->grid.five = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/six.bmp");
    textures->grid.six = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/seven.bmp");
    textures->grid.seven = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/eight.bmp");
    textures->grid.eight = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("./textures/grid/empty.bmp");
    textures->grid.empty = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/flag.bmp");
    textures->grid.flag = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/block.bmp");
    textures->grid.block = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/bomb.bmp");
    textures->grid.mine = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/grid/blown_bomb.bmp");
    textures->grid.blown_mine = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("./textures/counter/zero.bmp");
    textures->counter.zero = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/one.bmp");
    textures->counter.one = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/two.bmp");
    textures->counter.two = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/three.bmp");
    textures->counter.three = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/four.bmp");
    textures->counter.four = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/five.bmp");
    textures->counter.five = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/six.bmp");
    textures->counter.six = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/seven.bmp");
    textures->counter.seven = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/eight.bmp");
    textures->counter.eight = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/counter/nine.bmp");
    textures->counter.nine = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("./textures/frame_easy.bmp");
    textures->frame_easy = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("./textures/mood_neutral.bmp");
    textures->smiley_neutral = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void init_block(blocks square[][100]){
    for(int i = 0; i < gameWidth; i++){
        for(int j = 0; j < gameHeight; j++){
            square[i][j].hasmine = false;
            square[i][j].hasflag = false;
            square[i][j].isshown = false;
        }
    }

    int totalmines = 0;
    int i, j, randnum = 0;

    while(totalmines < (0.17 * (gameWidth * gameHeight))){
        srand(time(NULL) * totalmines * randnum * SDL_GetTicks64());
        i = rand() % gameWidth;
        j = rand() % gameHeight;
        randnum = rand() % 1000;
        if(!square[i][j].hasmine){
            square[i][j].hasmine = true;
            totalmines++;
        }
    }

    for(int i = 0; i < gameWidth; i++){
        for(int j = 0; j < gameHeight; j++){
            int minesAround = 0;
            for(int a = i - 1; a <= i + 1; a++){
                for(int b = j - 1; b <= j + 1; b++){
                    if((a < gameWidth && a >= 0) && (b < gameHeight && b >= 0)){
                        if(square[a][b].hasmine){
                            minesAround++;
                        }
                    }
                }
            }
            square[i][j].minesnear = minesAround;
        }
    }
}

void reveal_area(blocks square[][100], int i, int j){
    square[i][j].isshown = true;

    if(square[i][j].minesnear == 0){
        for(int a = i - 1; a <= i + 1; a++){
            for(int b = j - 1; b <= j + 1; b++){
                if(!square[a][b].hasmine && !square[a][b].isshown && a >= 0 && a < gameWidth && b >= 0 && b < gameHeight && !square[a][b].hasflag){
                    square[a][b].isshown = true;
                    if(square[a][b].minesnear == 0){
                        reveal_area(square, a, b);
                    }
                }
            }
        }
    }
}

void show_grid(blocks square[][100], gameTxtr *txtr, SDL_Renderer *renderer){
    for(int i = 0; i < gameWidth; i++){
        for(int j = 0; j < gameHeight; j++){
            if(square[i][j].isshown){
                if(square[i][j].hasmine){
                    SDL_RenderCopy(renderer, txtr->grid.blown_mine, NULL, &square[i][j].quadrant);
                    if(square[i][j].hasflag){
                        SDL_RenderCopy(renderer, txtr->grid.mine, NULL, &square[i][j].quadrant);
                    }
                }
                if(!square[i][j].hasmine){
                    if(square[i][j].minesnear > 0){
                        switch(square[i][j].minesnear){
                            case 1: SDL_RenderCopy(renderer, txtr->grid.one, NULL, &square[i][j].quadrant);
                                    break;
                            case 2: SDL_RenderCopy(renderer, txtr->grid.two, NULL, &square[i][j].quadrant);
                                    break;
                            case 3: SDL_RenderCopy(renderer, txtr->grid.three, NULL, &square[i][j].quadrant);
                                    break;
                            case 4: SDL_RenderCopy(renderer, txtr->grid.four, NULL, &square[i][j].quadrant);
                                    break;
                            case 5: SDL_RenderCopy(renderer, txtr->grid.five, NULL, &square[i][j].quadrant);
                                    break;
                            case 6: SDL_RenderCopy(renderer, txtr->grid.six, NULL, &square[i][j].quadrant);
                                    break;
                            case 7: SDL_RenderCopy(renderer, txtr->grid.seven, NULL, &square[i][j].quadrant);
                                    break;
                            case 8: SDL_RenderCopy(renderer, txtr->grid.eight, NULL, &square[i][j].quadrant);
                                    break;
                        }
                    }
                    else{
                        SDL_RenderCopy(renderer, txtr->grid.empty, NULL, &square[i][j].quadrant);
                    }
                }          
            }

            else{
                SDL_RenderCopy(renderer, txtr->grid.block, NULL, &square[i][j].quadrant);
                if(square[i][j].hasflag){
                    SDL_RenderCopy(renderer, txtr->grid.flag, NULL, &square[i][j].quadrant);
                }
            }   
        }
    }
    SDL_RenderPresent(renderer);
}

void show_time(SDL_Renderer *renderer, gameTxtr *textures, tmrDisp *timer, int secs){
    int unity, decimal, hundred;

    hundred = secs / 100;
    secs -= (hundred * 100);
    decimal = secs / 10;
    secs -= (decimal * 10);
    unity = secs;

    switch(unity){
        case 0:
        SDL_RenderCopy(renderer, textures->counter.zero, NULL, &(timer->slot3));
        break;
        case 1:
        SDL_RenderCopy(renderer, textures->counter.one, NULL, &(timer->slot3));
        break;
        case 2:
        SDL_RenderCopy(renderer, textures->counter.two, NULL, &(timer->slot3));
        break;
        case 3:
        SDL_RenderCopy(renderer, textures->counter.three, NULL, &(timer->slot3));
        break;
        case 4:
        SDL_RenderCopy(renderer, textures->counter.four, NULL, &(timer->slot3));
        break;
        case 5:
        SDL_RenderCopy(renderer, textures->counter.five, NULL, &(timer->slot3));
        break;
        case 6:
        SDL_RenderCopy(renderer, textures->counter.six, NULL, &(timer->slot3));
        break;
        case 7:
        SDL_RenderCopy(renderer, textures->counter.seven, NULL, &(timer->slot3));
        break;
        case 8:
        SDL_RenderCopy(renderer, textures->counter.eight, NULL, &(timer->slot3));
        break;
        case 9:
        SDL_RenderCopy(renderer, textures->counter.nine, NULL, &(timer->slot3));
    }
    switch(decimal){
        case 0:
        SDL_RenderCopy(renderer, textures->counter.zero, NULL, &(timer->slot2));
        break;
        case 1:
        SDL_RenderCopy(renderer, textures->counter.one, NULL, &(timer->slot2));
        break;
        case 2:
        SDL_RenderCopy(renderer, textures->counter.two, NULL, &(timer->slot2));
        break;
        case 3:
        SDL_RenderCopy(renderer, textures->counter.three, NULL, &(timer->slot2));
        break;
        case 4:
        SDL_RenderCopy(renderer, textures->counter.four, NULL, &(timer->slot2));
        break;
        case 5:
        SDL_RenderCopy(renderer, textures->counter.five, NULL, &(timer->slot2));
        break;
        case 6:
        SDL_RenderCopy(renderer, textures->counter.six, NULL, &(timer->slot2));
        break;
        case 7:
        SDL_RenderCopy(renderer, textures->counter.seven, NULL, &(timer->slot2));
        break;
        case 8:
        SDL_RenderCopy(renderer, textures->counter.eight, NULL, &(timer->slot2));
        break;
        case 9:
        SDL_RenderCopy(renderer, textures->counter.nine, NULL, &(timer->slot2));
    }
    switch(hundred){
        case 0:
        SDL_RenderCopy(renderer, textures->counter.zero, NULL, &(timer->slot1));
        break;
        case 1:
        SDL_RenderCopy(renderer, textures->counter.one, NULL, &(timer->slot1));
        break;
        case 2:
        SDL_RenderCopy(renderer, textures->counter.two, NULL, &(timer->slot1));
        break;
        case 3:
        SDL_RenderCopy(renderer, textures->counter.three, NULL, &(timer->slot1));
        break;
        case 4:
        SDL_RenderCopy(renderer, textures->counter.four, NULL, &(timer->slot1));
        break;
        case 5:
        SDL_RenderCopy(renderer, textures->counter.five, NULL, &(timer->slot1));
        break;
        case 6:
        SDL_RenderCopy(renderer, textures->counter.six, NULL, &(timer->slot1));
        break;
        case 7:
        SDL_RenderCopy(renderer, textures->counter.seven, NULL, &(timer->slot1));
        break;
        case 8:
        SDL_RenderCopy(renderer, textures->counter.eight, NULL, &(timer->slot1));
        break;
        case 9:
        SDL_RenderCopy(renderer, textures->counter.nine, NULL, &(timer->slot1));
    }
}

int main(int agrc, char *argv[]){

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    if(window == NULL){
        printf("Could not initialize window. Error: %s", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;

    gameTxtr textures;
    init_textures(&textures, renderer);

    bool leftdown = false;
    bool rightdown = false;
    bool r_down = false;
    bool gameOver = false;
    int mousex, mousey;

    int lastWIDTH = 0, lastHEIGHT = 0;
    int lastGameWidth = 0, lastGameHeight = 0;

    int smileyMood = 0;
    int difficulty = 1;
    int totalMines;
    minePos mineTracker[100 * 100];
    int mineTrackerSize;
    
    int gameStartTime = 0;
    int timePassed = 0;

    SDL_Rect mouseRect;
    mouseRect.w = 1;
    mouseRect.h = 1;
    SDL_Rect intersection;
    SDL_Rect smileyRect;

    mcDisp mineCounterRect;
    tmrDisp timerRect;

    SDL_Rect frame;
    int blocksize;
    blocks block[100][100];
    
    bool running = true;
    while(running){
        leftdown = false;
        rightdown = false;
        r_down = false;

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
                SDL_DestroyWindow(window);
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    leftdown = true;
                }
                else if(event.button.button == SDL_BUTTON_RIGHT){
                    rightdown = true;
                }
            }
            if(event.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mousex, &mousey);
                mouseRect.x = mousex;
                mouseRect.y = mousey;
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_r){
                    r_down = true;
                }
            }
        }

        SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
        if(WIDTH != lastWIDTH || HEIGHT != lastHEIGHT || gameWidth != lastGameWidth || gameHeight != lastGameHeight)
        {
            blocksize = (400 - 34)/gameWidth;
            if(difficulty == 1){
                frame.w = blocksize * gameWidth;
                frame.h = blocksize * gameHeight;
                frame.x = WIDTH/2 - frame.w/2;
                frame.y = HEIGHT - frame.h - frame.x;

                smileyRect.w = 7*(frame.y - frame.x * 2)/8;
                smileyRect.h = 7*(frame.y - frame.x * 2)/8;
                smileyRect.x = WIDTH/2 - smileyRect.w/2;
                smileyRect.y = frame.y - frame.y/2 - smileyRect.h/2;

                mineCounterRect.frame.w = 40 * 3;
                mineCounterRect.frame.h = 60;
                mineCounterRect.frame.x = (smileyRect.x + frame.x)/2 - mineCounterRect.frame.w/2;
                mineCounterRect.frame.y = frame.y - frame.y/2 - mineCounterRect.frame.h/2;

                mineCounterRect.slot1 = mineCounterRect.frame;
                mineCounterRect.slot1.w = 40;
                mineCounterRect.slot2 = mineCounterRect.slot1;
                mineCounterRect.slot2.x += mineCounterRect.slot1.w;
                mineCounterRect.slot3 = mineCounterRect.slot2;
                mineCounterRect.slot3.x += mineCounterRect.slot2.w;

                timerRect.frame = mineCounterRect.frame;
                timerRect.frame.x = smileyRect.x + smileyRect.w + (smileyRect.x - (mineCounterRect.frame.x + mineCounterRect.frame.w));
                timerRect.slot1 = timerRect.frame;
                timerRect.slot1.w = 40;
                timerRect.slot2 = timerRect.slot1;
                timerRect.slot2.x += timerRect.slot1.w;
                timerRect.slot3 = timerRect.slot2;
                timerRect.slot3.x += timerRect.slot2.w;
            }
            for(int i = 0; i < gameWidth; i++){
                for(int j = 0; j < gameHeight; j++){
                    block[i][j].quadrant.w = blocksize;
                    block[i][j].quadrant.h = blocksize;
                    block[i][j].quadrant.x = frame.x + block[i][j].quadrant.w * i;
                    block[i][j].quadrant.y = frame.y + block[i][j].quadrant.h * j;
                }
            }       

            lastWIDTH = WIDTH;
            lastHEIGHT = HEIGHT;
            lastGameWidth = gameWidth;
            lastGameHeight = gameHeight;
        }
        
        if(r_down){
            init_block(block);
            gameOver = false;
            gameStartTime = time(NULL);
            for(int a = 0; a < (gameWidth * gameHeight); a++){
                mineTracker[a].x = 0;
                mineTracker[a].y = 0;
            }
            //SDL_SetWindowSize(window, frame.w, frame.h);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if(!gameOver){
            timePassed = time(NULL) - gameStartTime;
            if(timePassed > 5){
                gameOver = true;
            }
            for(int i = 0; i < gameWidth; i++)
            {
                for(int j = 0; j < gameHeight; j++)
                {
                    if(SDL_IntersectRect(&mouseRect, &block[i][j].quadrant, &intersection)){
                        if(leftdown && !block[i][j].hasflag){
                            block[i][j].isshown = true;
                            if(block[i][j].hasmine){
                                block[i][j].isshown = true;
                                totalMines = 0;
                                gameOver = true;
                                for(int a = 0; a < gameWidth; a++){
                                    for(int b = 0; b < gameHeight; b++){
                                        if(block[a][b].hasmine){
                                            mineTracker[totalMines].x = a;
                                            mineTracker[totalMines].y = b;
                                            totalMines++;
                                        }
                                    }
                                }
                                mineTrackerSize = totalMines;
                            }
                            else{
                                reveal_area(block, i, j);
                            }
                        }
                        if(rightdown && !block[i][j].isshown){
                            if(block[i][j].hasflag){
                                block[i][j].hasflag = false;
                            }
                            else{
                                block[i][j].hasflag = true;
                            }
                        }
                    }
                }
            }  
        }
        else{
            int num = 5;
            if(totalMines > 1)
            {
                srand(time(NULL) * SDL_GetTicks64());
                while(mineTracker[num].x == 0 && mineTracker[num].y == 0){
                    num = rand() % (mineTrackerSize);
                }
                block[mineTracker[num].x][mineTracker[num].y].isshown = true;
                mineTracker[num].x = 0;
                mineTracker[num].y = 0;   
                if(totalMines > mineTrackerSize - 2){
                    SDL_Delay(rand() % 700);
                }
                else if(totalMines > mineTrackerSize/2){
                    SDL_Delay(rand() % 200);
                }
                else{
                    SDL_Delay(rand() % 50);
                }
                totalMines--;
            }
        }
        SDL_RenderCopy(renderer, textures.frame_easy, NULL, NULL);
        SDL_RenderCopy(renderer, textures.smiley_neutral, NULL, &smileyRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderDrawRect(renderer, &mineCounterRect.frame);
        SDL_RenderDrawRect(renderer, &mineCounterRect.slot1);
        SDL_RenderDrawRect(renderer, &mineCounterRect.slot2);
        SDL_RenderDrawRect(renderer, &mineCounterRect.slot3);
        
        SDL_RenderDrawRect(renderer, &timerRect.frame);
        SDL_RenderDrawRect(renderer, &timerRect.slot1);
        SDL_RenderDrawRect(renderer, &timerRect.slot2);
        SDL_RenderDrawRect(renderer, &timerRect.slot3);
        show_time(renderer, &textures, &timerRect, timePassed);

        show_grid(block, &textures, renderer);
    }

    return 0;
}