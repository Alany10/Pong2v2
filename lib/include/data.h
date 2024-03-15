#ifndef data_h
#define data_h

#define MAX_PADDLES 4

enum gameState{START, ONGOING, GAME_OVER};
typedef enum gameState GameState;

enum clientCommand{READY, UP, DOWN};
typedef enum clientCommand ClientCommand;

struct scoreData{
    int leftScore, rightScore;
};
typedef struct scoreData ScoreData;

struct clientData{
    ClientCommand command;
    int playerNumber;
};
typedef struct clientData ClientData;

struct ballData{
    float x, y, xv, yv;
};
typedef struct ballData BallData;

struct paddleData{
    float x, y, xv, yv;
};
typedef struct paddleData PaddleData;   

struct serverData{
    PaddleData paddle1, paddle2, paddle3, paddle4;
    BallData ball;
    int playerNr;
    GameState gState;
    ScoreData score;
};
typedef struct serverData ServerData;

#endif