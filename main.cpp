#include <stdio.h>
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>

int WIDTH = 600, HEIGHT = 600;
int gameWidth = 20, gameHeight = 20;

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
};
typedef grid_textures gameTxtr;

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
                    SDL_RenderCopy(renderer, txtr->blown_mine, NULL, &square[i][j].quadrant);
                    if(square[i][j].hasflag){
                        SDL_RenderCopy(renderer, txtr->mine, NULL, &square[i][j].quadrant);
                    }
                }
                if(!square[i][j].hasmine){
                    if(square[i][j].minesnear > 0){
                        switch(square[i][j].minesnear){
                            case 1: SDL_RenderCopy(renderer, txtr->one, NULL, &square[i][j].quadrant);
                                    break;
                            case 2: SDL_RenderCopy(renderer, txtr->two, NULL, &square[i][j].quadrant);
                                    break;
                            case 3: SDL_RenderCopy(renderer, txtr->three, NULL, &square[i][j].quadrant);
                                    break;
                            case 4: SDL_RenderCopy(renderer, txtr->four, NULL, &square[i][j].quadrant);
                                    break;
                            case 5: SDL_RenderCopy(renderer, txtr->five, NULL, &square[i][j].quadrant);
                                    break;
                            case 6: SDL_RenderCopy(renderer, txtr->six, NULL, &square[i][j].quadrant);
                                    break;
                            case 7: SDL_RenderCopy(renderer, txtr->seven, NULL, &square[i][j].quadrant);
                                    break;
                            case 8: SDL_RenderCopy(renderer, txtr->eight, NULL, &square[i][j].quadrant);
                                    break;
                        }
                    }
                    else{
                        SDL_RenderCopy(renderer, txtr->empty, NULL, &square[i][j].quadrant);
                    }
                }          
            }

            else{
                SDL_RenderCopy(renderer, txtr->block, NULL, &square[i][j].quadrant);
                if(square[i][j].hasflag){
                    SDL_RenderCopy(renderer, txtr->flag, NULL, &square[i][j].quadrant);
                }
            }   
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int agrc, char *argv[]){

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    if(window == NULL){
        printf("Could not initialize window. Error: %s", SDL_GetError());
        return 1;
    }
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;
    SDL_DisplayMode *mode;

    gameTxtr textures;

    SDL_Surface *surface = SDL_LoadBMP("./textures/one.bmp");
    textures.one = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/two.bmp");
    textures.two = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/three.bmp");
    textures.three = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/four.bmp");
    textures.four = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/five.bmp");
    textures.five = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/six.bmp");
    textures.six = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/seven.bmp");
    textures.seven = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/eight.bmp");
    textures.eight = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("./textures/empty.bmp");
    textures.empty = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/flag.bmp");
    textures.flag = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/block.bmp");
    textures.block = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/bomb.bmp");
    textures.mine = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = SDL_LoadBMP("./textures/blown_bomb.bmp");
    textures.blown_mine = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    bool leftdown = false;
    bool rightdown = false;
    bool r_down = false;
    bool up_arrow_down;
    bool down_arrow_down;
    bool right_arrow_down = false;
    bool left_arrow_down = false;
    bool gameOver = false;
    int mousex, mousey;
    int lastWIDTH = 0, lastHEIGHT = 0;
    int lastGameWidth = 0, lastGameHeight = 0;
    int smileyMood = 0;
    int totalMines;
    int centerX, centerY;

    SDL_Rect mouseRect;
    mouseRect.w = 1;
    mouseRect.h = 1;

    SDL_Rect intersection;

    SDL_Rect triggerMine;
    triggerMine.w = 1;
    triggerMine.h = 1;
    SDL_Rect smileyRect;
    SDL_Rect frame;
    int blocksize;
    blocks block[100][100];
    
    bool running = true;
    while(running){
        leftdown = false;
        rightdown = false;
        r_down = false;
        up_arrow_down = false;
        down_arrow_down = false;
        right_arrow_down = false;
        left_arrow_down = false;

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
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
        if(WIDTH != lastWIDTH || HEIGHT != lastHEIGHT || gameWidth != lastGameWidth || gameHeight != lastGameHeight){
            if(gameWidth > gameHeight){
                blocksize = (WIDTH - WIDTH/10)/gameWidth;
            }
            else{
                blocksize = (HEIGHT - HEIGHT/10)/gameHeight;
            }
            frame.h = blocksize * gameHeight;
            frame.w = blocksize * gameWidth;
            frame.x = WIDTH/2 - frame.w/2;
            frame.y = HEIGHT/2 - frame.h/2;

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
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &frame);  

        for(int i = 0; i < gameWidth; i++)
        {
            for(int j = 0; j < gameHeight; j++)
            {
                if(SDL_IntersectRect(&mouseRect, &block[i][j].quadrant, &intersection)){
                    if(leftdown && !block[i][j].hasflag){
                        block[i][j].isshown = true;
                        if(block[i][j].hasmine){
                            uint32_t randDelay;
                            totalMines = 0;
                            gameOver = true;
                            centerX = i;
                            centerY = j;
                            for(int a = 0; a < gameWidth; a++){
                                for(int b = 0; b < gameHeight; b++){
                                    if(block[a][b].hasmine){
                                        block[a][b].isshown = true;
                                        totalMines++;
                                        show_grid(block, &textures, renderer);
                                    }
                                }
                            }
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

        show_grid(block, &textures, renderer);
    }

    return 0;
}