#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include "../../lib/include/ball.h"
#include "../../lib/include/paddle.h"
#include "../../lib/include/music.h"
#include "../../lib/include/text.h"
#include "../../lib/include/data.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 667

struct game
{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackground, *pMainMenuBackground;
    Paddle *pPaddle1, *pPaddle2, *pPaddle3, *pPaddle4;
    int nrOfPaddles;
    Ball *pBall;
    TTF_Font *pFont;
    Text *pScoreText, *pLeftScoreText, *pRightScoreText, *pStartText, *pOverText, *pLeftWinText, *pRightWinText;
    int rightScore, leftScore;
    GameState state;
    UDPsocket pSocket;
    UDPpacket *pPacket;
    IPaddress clients[MAX_PADDLES];
    int nrOfClients;
    ServerData sData;
};
typedef struct game Game;

int initiate(Game *pGame);
void runGame(Game *pGame);
void close(Game *pGame);
void collision(Game *pGame);
void add(IPaddress address, IPaddress clients[],int *pNrOfClients);
void setUpGame(Game *pGame);
void sendGameData(Game *pGame);
void executeCommand(Game *pGame, ClientData cData);

int main(int argc, char** argv)
{
    Game g={0};
    if(!initiate(&g)) return 1; 
    runGame(&g);
    close(&g);
    return 0;
}

int initiate(Game *pGame)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 0;
    }
    if(TTF_Init()!=0){
    printf("Error: %s\n",TTF_GetError());
    SDL_Quit();
    return 0;
    }
    if (SDLNet_Init() !=0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        TTF_Quit();
        SDL_Quit();
		return 0;
	}

    pGame->pWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pGame->pWindow) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (!pGame->pRenderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }

    SDL_Surface *pSurface = IMG_Load("../lib/resources/pongtabletexture.png");
    if (!pSurface) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        close(pGame);
        return 0;
    }
    pGame->pBackground = SDL_CreateTextureFromSurface(pGame->pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pGame->pBackground) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }

    SDL_Surface *pMainMenuSurface = IMG_Load("../lib/resources/serverMainMenu.jpg");
    if (!pMainMenuSurface)
    {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        close(pGame);
        return 0;
    }

    pGame->pMainMenuBackground = SDL_CreateTextureFromSurface(pGame->pRenderer, pMainMenuSurface);
    SDL_FreeSurface(pMainMenuSurface);
    if (!pGame->pMainMenuBackground)
    {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    
    pGame->pFont = TTF_OpenFont("../lib/resources/arial.ttf", 40);
    if(!pGame->pFont){
        printf("Error: %s\n",TTF_GetError());
        close(pGame);
        return 0;
    }

    if (!(pGame->pSocket = SDLNet_UDP_Open(2000))){
		printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    if (!(pGame->pPacket = SDLNet_AllocPacket(512))){
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		close(pGame);
        return 0;
	}
    
    pGame->pPaddle1 = createPaddle(30, 100, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pPaddle2 = createPaddle(30, 400, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pPaddle3 = createPaddle(WINDOW_WIDTH-50, 100, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pPaddle4 = createPaddle(WINDOW_WIDTH-50, 400, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->nrOfPaddles = MAX_PADDLES;
    pGame->pBall = createBall(WINDOW_WIDTH, WINDOW_HEIGHT, pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pOverText = createText(pGame->pRenderer, 255, 0, 0, pGame->pFont, "GAME OVER", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pGame->pStartText = createText(pGame->pRenderer,255,255,255,pGame->pFont,"Waiting for clients",WINDOW_WIDTH-200,WINDOW_HEIGHT-50);
    pGame->pScoreText = createText(pGame->pRenderer, 255, 0, 0, pGame->pFont, "SCORE", WINDOW_WIDTH/2, 30);// skapa texten
    pGame->pLeftWinText = createText(pGame->pRenderer, 255, 255, 255, pGame->pFont, "Left Team wins", WINDOW_WIDTH / 2, 30);
    pGame->pRightWinText = createText(pGame->pRenderer, 255, 255, 255, pGame->pFont, "Right Team wins", WINDOW_WIDTH / 2, 30);

    if(!pGame->pPaddle1 || !pGame->pPaddle2 || !pGame->pPaddle3 || !pGame->pPaddle4){
        printf("Error: %s\n",SDL_GetError());
        close(pGame);
        return 0;
    }

    pGame->state = START;
    pGame->nrOfClients = 0;

    return 1;

}

void runGame(Game *pGame)
{
    pGame->leftScore=pGame->rightScore=0;
    bool isRunning = true;
    SDL_Event event;
    ClientData cData;
    bool isMenuMusicPlaying = false;
    bool isGameOver = false;

    while (isRunning) {
        switch (pGame->state){
            case ONGOING:
                if (isMenuMusicPlaying) {
                    // Stop menu music if playing
                    Mix_HaltMusic();
                    isMenuMusicPlaying = false;
                    PlayMusic("../lib/resources/Santana - Maria Maria (sped up) Lyrics  she living the life just like a movie star.mp3");
                }
            sendGameData(pGame);
            while(SDLNet_UDP_Recv(pGame->pSocket, pGame->pPacket) == 1){
                memcpy(&cData, pGame->pPacket->data, sizeof(ClientData));
                executeCommand(pGame,cData);
            }
            if(SDL_PollEvent(&event)) if(event.type==SDL_QUIT) isRunning = false;
            SDL_RenderClear(pGame->pRenderer);
            SDL_RenderCopy(pGame->pRenderer, pGame->pBackground, NULL, NULL);
            updatePaddle(pGame->pPaddle1);
            updatePaddle(pGame->pPaddle2);
            updatePaddle(pGame->pPaddle3);
            updatePaddle(pGame->pPaddle4);
            updateBall(pGame->pBall);
            wallCollision(pGame->pBall,&pGame->leftScore, &pGame->rightScore);
            collision(pGame);
            drawPaddle(pGame->pPaddle1);
            drawPaddle(pGame->pPaddle2);
            drawPaddle(pGame->pPaddle3);
            drawPaddle(pGame->pPaddle4);
            drawBall(pGame->pBall);
            //score
            char leftScoreNumberStr[10];
            snprintf(leftScoreNumberStr, 10, "%d", pGame->leftScore);
            pGame->pLeftScoreText = createText(pGame->pRenderer, 255, 255, 255, pGame->pFont, leftScoreNumberStr, (WINDOW_WIDTH/2) - 50, 70);

            //Update right player score text
            char rightScoreNumberStr[10];
            snprintf(rightScoreNumberStr, 10, "%d", pGame->rightScore);
            pGame->pRightScoreText = createText(pGame->pRenderer, 255, 255, 255, pGame->pFont, rightScoreNumberStr, (WINDOW_WIDTH/2) + 50, 70);
            drawText(pGame->pScoreText);// skriva ut texten pGame->
            drawText(pGame->pLeftScoreText);
            drawText(pGame->pRightScoreText);
            destroyText(pGame->pRightScoreText);
            destroyText(pGame->pLeftScoreText);
            SDL_RenderPresent(pGame->pRenderer);
            if (pGame->leftScore == 5 || pGame->rightScore == 5) {
                pGame->state = GAME_OVER; 
            }
            break;
            case GAME_OVER:
                SDL_RenderClear(pGame->pRenderer);
                SDL_RenderCopy(pGame->pRenderer, pGame->pBackground, NULL, NULL);
                drawText(pGame->pOverText);
                if(pGame->leftScore>pGame->rightScore){
                    drawText(pGame->pLeftWinText);
                }
                else{
                    drawText(pGame->pRightWinText);
                } 
                SDL_RenderPresent(pGame->pRenderer);
                SDL_Delay(3000);
                sendGameData(pGame);
                if(pGame->nrOfClients==MAX_PADDLES) pGame->nrOfClients = 0;
                pGame->state = START;
                break;
            case START:
                pGame->leftScore=pGame->rightScore=0;
                if (!isMenuMusicPlaying) {
                    // Play menu music if not already playing
                    menuMusic("../lib/resources/02 Firelink Shrine.mp3");
                    isMenuMusicPlaying = true;
                }
                SDL_RenderCopy(pGame->pRenderer, pGame->pMainMenuBackground, NULL, NULL);
                drawText(pGame->pStartText);
                SDL_RenderPresent(pGame->pRenderer);
                if(SDL_PollEvent(&event) && event.type==SDL_QUIT) isRunning = false;
                if(SDLNet_UDP_Recv(pGame->pSocket,pGame->pPacket)==1){
                    add(pGame->pPacket->address,pGame->clients,&(pGame->nrOfClients));
                    if(pGame->nrOfClients==MAX_PADDLES) setUpGame(pGame);
                }
            break;
        }
        //SDL_SetRenderDrawColor(pGame->pRenderer, 255, 0, 0, 255);
    }
    if (isMenuMusicPlaying) {
        Mix_HaltMusic();
    }
}



void close(Game *pGame)
{
    if(pGame->pPaddle1) destroyPaddle(pGame->pPaddle1);
    if(pGame->pPaddle2) destroyPaddle(pGame->pPaddle2);
    if(pGame->pPaddle3) destroyPaddle(pGame->pPaddle3);
    if(pGame->pPaddle4) destroyPaddle(pGame->pPaddle4);
    if(pGame->pBall) destroyBall(pGame->pBall);
    if(pGame->pOverText) destroyText(pGame->pOverText);
    if(pGame->pStartText) destroyText(pGame->pStartText); 
    if(pGame->pRightScoreText) destroyText(pGame->pRightScoreText);
    if(pGame->pLeftScoreText) destroyText(pGame->pLeftScoreText);
    if(pGame->pScoreText) destroyText(pGame->pScoreText);

    if(pGame->pBackground) SDL_DestroyTexture(pGame->pBackground);
    if(pGame->pRenderer) SDL_DestroyRenderer(pGame->pRenderer);
    if(pGame->pWindow) SDL_DestroyWindow(pGame->pWindow);

    if(pGame->pFont) TTF_CloseFont(pGame->pFont);
    if(pGame->pPacket) SDLNet_FreePacket(pGame->pPacket);
	if(pGame->pSocket) SDLNet_UDP_Close(pGame->pSocket);

    //Mix_FreeChunk(wallBallCollision);
    TTF_Quit();    
    IMG_Quit();
    SDL_Quit();
}

void collision(Game *pGame){
        if (SDL_HasIntersection(&(pGame->pBall->ballRect), &(pGame->pPaddle1->paddleRect)) || SDL_HasIntersection(&(pGame->pBall->ballRect), &(pGame->pPaddle2->paddleRect))) {
            pGame->pBall->xv = -pGame->pBall->xv;
            pGame->pBall->x +=15;
        } 
        if (SDL_HasIntersection(&(pGame->pBall->ballRect), &(pGame->pPaddle3->paddleRect)) || SDL_HasIntersection(&(pGame->pBall->ballRect), &(pGame->pPaddle4->paddleRect))) {
            pGame->pBall->xv = -pGame->pBall->xv;
            pGame->pBall->x -=15;
        }
}
void add(IPaddress address, IPaddress clients[],int *pNrOfClients){
	for(int i=0;i<*pNrOfClients;i++) 
    if(address.host==clients[i].host &&address.port==clients[i].port) 
    return;
	clients[*pNrOfClients] = address;
	(*pNrOfClients)++;
}

void setUpGame(Game *pGame){
    resetPaddle(pGame->pPaddle1, 1);
    resetPaddle(pGame->pPaddle2, 2);
    resetPaddle(pGame->pPaddle3, 3);
    resetPaddle(pGame->pPaddle4, 4);
    resetBall(pGame->pBall);

    pGame->nrOfPaddles = MAX_PADDLES;
    pGame->state = ONGOING;
}

void sendGameData(Game *pGame){
    pGame->sData.gState = pGame->state;
    getBallSendData(pGame->pBall,&(pGame->sData.ball));
    getPaddleSendData(pGame->pPaddle1, &(pGame->sData.paddle1));
    getPaddleSendData(pGame->pPaddle2, &(pGame->sData.paddle2));
    getPaddleSendData(pGame->pPaddle3, &(pGame->sData.paddle3));
    getPaddleSendData(pGame->pPaddle4, &(pGame->sData.paddle4));
    pGame->sData.score.leftScore=pGame->leftScore;
    pGame->sData.score.rightScore=pGame->rightScore;

    for(int i=0;i<MAX_PADDLES;i++){
        pGame->sData.playerNr = i;
        memcpy(pGame->pPacket->data, &(pGame->sData), sizeof(ServerData));
		pGame->pPacket->len = sizeof(ServerData);
        pGame->pPacket->address = pGame->clients[i];
		SDLNet_UDP_Send(pGame->pSocket,-1,pGame->pPacket);
    }
}

void executeCommand(Game *pGame, ClientData cData){
    switch (cData.command)
    {
        case UP:
            if(cData.playerNumber == 1){
                up(pGame->pPaddle1, cData.playerNumber);
            }
            else if(cData.playerNumber == 2){
                up(pGame->pPaddle2, cData.playerNumber);
            }
            else if(cData.playerNumber == 3){
                up(pGame->pPaddle3, cData.playerNumber);
            }
            else if(cData.playerNumber == 4){
                up(pGame->pPaddle4, cData.playerNumber);
            }
            break;
        case DOWN:
            if(cData.playerNumber == 1){
                down(pGame->pPaddle1, cData.playerNumber);
            }
            else if(cData.playerNumber == 2){
                down(pGame->pPaddle2, cData.playerNumber);
            }
            else if(cData.playerNumber == 3){
                down(pGame->pPaddle3, cData.playerNumber);
            }
            else if(cData.playerNumber == 4){
                down(pGame->pPaddle4, cData.playerNumber);
            }
            break;
    }
}
