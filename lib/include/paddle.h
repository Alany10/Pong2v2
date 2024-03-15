#ifndef paddle_h
#define paddle_h
#include <stdbool.h>
#include "../include/data.h"

#define PADDLE_SPEED 7

typedef struct paddle Paddle;

Paddle *createPaddle(int x, int y, SDL_Renderer *pRenderer, int window_width, int window_height);
void drawPaddle(Paddle *pPaddle);
void destroyPaddle(Paddle *pPaddle);
void getPaddleSendData(Paddle *pPaddle, PaddleData *pPaddleData);
void up(Paddle *pPaddle, int playerNumber);
void down(Paddle *pPaddle, int playerNumber);
void updatePaddle(Paddle *pPaddle);
void resetPaddle(Paddle *pPaddle, int playerNumber);
void updatePaddleWithRecievedData(Paddle *pPaddle, PaddleData *pPaddleData);

struct paddle
{
    int x, y, window_width, window_height, angel;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect paddleRect;

};

#endif 