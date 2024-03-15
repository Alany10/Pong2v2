#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdbool.h>
#include "../include/paddle.h"
#include "../include/data.h"

Paddle *createPaddle(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Paddle *pPaddle = malloc(sizeof(struct paddle));
    pPaddle->window_width = window_width;
    pPaddle->window_height = window_height;
    pPaddle->angel = 0;
    pPaddle->pRenderer = pRenderer;

    SDL_Surface *pPaddleSurface = IMG_Load("../lib/resources/Paddle.png");
    if(!pPaddleSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    pPaddle->pTexture = SDL_CreateTextureFromSurface(pRenderer, pPaddleSurface);
    if(!pPaddle->pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(pPaddle->pTexture, NULL, NULL, &(pPaddle->paddleRect.w), &(pPaddle->paddleRect.h));
    pPaddle->paddleRect.w = 20;
    pPaddle->paddleRect.h = 100;
    pPaddle->x = x;
    pPaddle->y = y - pPaddle->paddleRect.h/2;
    pPaddle->paddleRect.x = pPaddle->x;
    pPaddle->paddleRect.y = pPaddle->y;

    return pPaddle;
}

void drawPaddle(Paddle *pPaddle)
{
    SDL_RenderCopyEx(pPaddle->pRenderer, pPaddle->pTexture, NULL, &(pPaddle->paddleRect), pPaddle->angel, NULL, SDL_FLIP_NONE);
}

void destroyPaddle(Paddle *pPaddle)
{
    SDL_DestroyTexture(pPaddle->pTexture);
    free(pPaddle);
}

void up(Paddle *pPaddle, int playerNumber){
    switch (playerNumber)
    {
    case 1: 
        pPaddle->y -= PADDLE_SPEED;
        if (pPaddle->y < 0 )
        {
            pPaddle->y = 0;
        }
        break;
    case 2: 
        pPaddle->y -= PADDLE_SPEED;
        if (pPaddle->y < pPaddle->window_height/2) 
        {
            pPaddle->y = pPaddle->window_height/2;
        }
        break;
    case 3:
        pPaddle->y -= PADDLE_SPEED;
        if (pPaddle->y < 0 ) 
        {
            pPaddle->y = 0;
        }
        break;
    case 4:
        pPaddle->y -= PADDLE_SPEED;
        if (pPaddle->y < pPaddle->window_height/2) 
        {
            pPaddle->y = pPaddle->window_height/2;
        }
    break;
    }
}

void down(Paddle *pPaddle, int playerNumber){
    switch (playerNumber)
    {
    case 1: 
        pPaddle->y += PADDLE_SPEED;
        if (pPaddle->y + pPaddle->paddleRect.h > pPaddle->window_height/2) 
        {
            pPaddle->y = pPaddle->window_height/2-pPaddle->paddleRect.h;
        }
        break;
    case 2: 
        pPaddle->y +=PADDLE_SPEED;
        if(pPaddle->y + pPaddle->paddleRect.h > pPaddle->window_height)
        {
            pPaddle->y = pPaddle->window_height - pPaddle->paddleRect.h;
        }
        break;
    case 3: 
        pPaddle->y += PADDLE_SPEED;
        if (pPaddle->y + pPaddle->paddleRect.h > pPaddle->window_height/2) 
        {
            pPaddle->y = pPaddle->window_height/2 - pPaddle->paddleRect.h;
        }
        break;
    case 4:
        pPaddle->y += PADDLE_SPEED;
        if(pPaddle->y + pPaddle->paddleRect.h > pPaddle->window_height)
        {
            pPaddle->y = pPaddle->window_height - pPaddle->paddleRect.h;
        }
        break;
    }
}

void getPaddleSendData(Paddle *pPaddle, PaddleData *pPaddleData){
    pPaddleData->x = pPaddle->x;
    pPaddleData->y = pPaddle->y;
}

void updatePaddle(Paddle *pPaddle){
    pPaddle->paddleRect.x=pPaddle->x;
    pPaddle->paddleRect.y=pPaddle->y;
}

void resetPaddle(Paddle *pPaddle, int playerNumber){
    switch (playerNumber)
    {
    case 1:
        pPaddle->x = 30;
        pPaddle->y = 100;
        break;
    case 2:
        pPaddle->x = 30;
        pPaddle->y = 400;
        break;
    case 3:
        pPaddle->x = pPaddle->window_width - 50;
        pPaddle->y = 100;
        break;
    case 4:
        pPaddle->x = pPaddle->window_width - 50;
        pPaddle->y = 400;
        break;
    }
}

void updatePaddleWithRecievedData(Paddle *pPaddle, PaddleData *pPaddleData){
    pPaddle->x = pPaddleData->x;
    pPaddle->y = pPaddleData->y;
}