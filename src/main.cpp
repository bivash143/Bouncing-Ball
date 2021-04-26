#include<iostream>
#include <stdbool.h>
#include </usr/include/SDL2/SDL.h>
#include "constant.h"

using namespace std;

//Globel Variables

bool game_is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int last_frame_time = 0;

//Game objects 

struct ball
{
    float x, y, width, height, vel_x, vel_y;
} ball, paddle;

//Function for initializing SDL Window

int initialize_window(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        cout<<"Error initializing SDL"<<endl;
        return false;
    }

    window = SDL_CreateWindow("Bouncing Ball Game",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN);

    if(!window){
        cout<<"Error in creating Window"<<endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        cout<<"Error in creating renderer"<<endl;
        return false;
    }
    
    return true;

}

//function that runs once at the begining of program

void setup()
{
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
    ball.vel_x = 300;
    ball.vel_y = 300;

    paddle.width = 100;
    paddle.height = 20;
    paddle.x = (WINDOW_WIDH/2) - (paddle.width/2);
    paddle.y = (WINDOW_WIDH/2) + 150;
    paddle.vel_x = 0;
    paddle.vel_y = 0;
}

//function to poll SDL event and process Keyboard input

void process_input()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        game_is_running = false;
        SDL_Quit();
        break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE){
            game_is_running = false;
            SDL_Quit();
        }
        if(event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = -400;
        if(event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = +400;
        break;
    case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = 0;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = 0;
        break;
    }

}

//function that update frames

void update(){

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks()-last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }

    float delta_time = (SDL_GetTicks() - last_frame_time)/(float) 1000;

    last_frame_time = SDL_GetTicks();

    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;
    paddle.x += paddle.vel_x * delta_time;
    paddle.y += paddle.vel_y * delta_time;

    if(ball.x <=0 || ball.x + ball.width >= WINDOW_WIDH)
        ball.vel_x = -ball.vel_x;
    if(ball.y <= 0)
        ball.vel_y = -ball.vel_y;

    if(ball.y + ball.height >= paddle.y && ball.x + ball.width >= paddle.x && ball.x <= paddle.x + paddle.width)
        ball.vel_y = -ball.vel_y;

    if(paddle.x <= 0 )
        paddle.x = 0;
    if(paddle.x >= WINDOW_WIDH - paddle.width)
        paddle.x = WINDOW_WIDH - paddle.width;

    if(ball.y + ball.height > WINDOW_HEIGHT){
        ball.x = WINDOW_WIDH / 2;
        ball.y = 0;
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //draw rectangle ball
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    //Draw recrtangle paddle
    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    SDL_RenderPresent(renderer);
}

//function to destroy SDL Window

void destroy_window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//main function

int main()
{
    int game_is_running = initialize_window();

    setup();
    while (game_is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    return 0;
}
