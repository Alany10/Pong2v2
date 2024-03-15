#ifndef BALL_H
#define BALL_H
#define BALL_SIZE 30
#define BALL_VELOCITY 6
#include "../include/data.h"

typedef struct ball Ball;

Ball *createBall(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void drawBall(Ball *pBall);
void updateBall(Ball *pBall);
void wallCollision(Ball *pBall, int *leftScore, int *rightScore);
void destroyBall(Ball *pBall);
void getBallSendData(Ball *pBall, BallData *pBallData);
void resetBall(Ball *pBall);
void updateBallWithRecievedData(Ball *pBall, BallData *pBallData);

struct ball{
    float x, y, xv, yv;
    int angle;
    int window_width, window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect ballRect;
};

#endif /* BALL_H */
