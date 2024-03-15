#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../include/ball.h"
#include "../include/data.h"

Ball *createBall(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height){
    Ball *pBall = malloc(sizeof(struct ball));
    pBall->xv=pBall->yv = BALL_VELOCITY;
    pBall->angle=0;
    pBall->window_width = window_width;
    pBall->window_height = window_height;
    SDL_Surface *pSurface = IMG_Load("../lib/resources/ball.png");
    if(!pSurface){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    pBall->pRenderer = pRenderer;
    pBall->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if(!pBall->pTexture){
        printf("Error: %s\n",SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(pBall->pTexture,NULL,NULL,&(pBall->ballRect.w),&(pBall->ballRect.h));
    pBall->ballRect.w = BALL_SIZE;
    pBall->ballRect.h = BALL_SIZE;
    pBall->x=(x-pBall->ballRect.w)/2;
    pBall->y=(y-pBall->ballRect.h)/2;
    pBall->ballRect.x = pBall->x;
    pBall->ballRect.y = pBall->y;

    return pBall;
}

void drawBall(Ball *pBall)
{
    pBall->ballRect.x = pBall->x;
    pBall->ballRect.y = pBall->y;
    SDL_RenderCopyEx(pBall->pRenderer, pBall->pTexture, NULL, &(pBall->ballRect), pBall->angle, NULL, SDL_FLIP_NONE);
}

void wallCollision(Ball *pBall, int *leftScore, int *rightScore)
{
    if(pBall->x + BALL_SIZE< 0 ){
        pBall->x = pBall->window_width/2;
        pBall->y = pBall->window_height/2;
        pBall->xv = -pBall->xv;
        (*rightScore)++; 
    }
    else if(pBall->x + BALL_SIZE > pBall->window_width)
    {
        pBall->x = pBall->window_width/2;
        pBall->y = pBall->window_height/2;
        pBall->xv = -pBall->xv;
        (*leftScore)++;
    }
    else if(pBall->y < 0){
        pBall->yv = -pBall->yv;
    }
    else if(pBall->y + BALL_SIZE > pBall->window_height){
        pBall->yv = -pBall->yv; 
    }
    
}

void updateBall(Ball *pBall){
    pBall->x+=pBall->xv;
    pBall->y+=pBall->yv;
}

void getBallSendData(Ball *pBall, BallData *pBallData){
    pBallData->xv = pBall->xv;
    pBallData->yv = pBall->yv;
    pBallData->x = pBall->x;
    pBallData->y = pBall->y;
}

void resetBall(Ball *pBall){
    pBall->x = (pBall->window_width -  pBall->ballRect.w) / 2;
    pBall->y = (pBall->window_height - pBall->ballRect.h) / 2;
}

void destroyBall(Ball *pBall)
{
    SDL_DestroyTexture(pBall->pTexture);
    free(pBall);
}

void updateBallWithRecievedData(Ball *pBall, BallData *pBallData){
    pBall->x = pBallData->x;
    pBall->y = pBallData->y;
}
