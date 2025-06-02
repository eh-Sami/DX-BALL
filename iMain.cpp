#include "iGraphics.h"
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define screenwidth 1100
#define screenheight 750
#define PI 3.14159265358979323846
#define initialBallAngle 80
#define powerupVelocity 3
#define BG 0
#define instructions 1
#define settings 2
#define credit 3
#define gameover 4
#define pause 5
#define livecount 4
#define playbutton 6
#define entername 7
#define highscore 8
#define theend 9
#define Exit 10
#define createlevelnum 10
#define numoflevel 9
#define first_level 0


int8_t scene = -1; // different scenes of the game
int8_t playBall = 0; // ball moving
int8_t lev = 0; // level of the game
int8_t bb = 22; // bombing brick
int8_t passiveMouseControl = 0;// to save the mouse position when clicked
int8_t mouseClicked = 0;
int8_t ex = 0; // 0 if there is no ball left on screen. screen moves to next level
int8_t showExplodeImage = 0;
int8_t powerupIncomingCnt = 0; // counts the number of bricks broken to calculate when to send a powerup
int8_t powerup_idx = -1;
int8_t lives = 4;
int8_t velocity = 11;
int8_t bigBall = 0;
int8_t throBall = 0;
int8_t fireBall = 0;
int8_t fire = 0;
int8_t bullet_idx = 0;
int8_t isResumed = 1;
int8_t selectedBrick = 0;
int8_t leftclick = 1;
int8_t playerName_idx = 0;
int8_t lowestscoer_idx = 0;
int8_t music = 1;
int8_t paddle_bounce = 1;// to store the index of the player with lowest score
int fireExplodeBrickCnt = 0;
int spacePressed = 0;
int score = 0;
int lowestscore = 10000000;
int waiting = 0;
double fps = 1.2;


double ballBounceX();
double ballBounceY();
double theta();
void change();
void resetEverything();
void explodeBrick(int, int);
void showBrick();
void bbchange();
void brickBounce();
void showPowerups();
void powerActivate(int);
void setPowerupPosition(int, int);
void showBall();
void explodeBrickAround(int, int);
void showFire();
void showgrid();
void showBrickOptions();
void r_and_w_highscore();
void sortScore();
void storeScore();


typedef struct
{
    int position;
    int x; 
    int y;
    int dx;
    int dy;
}pad;

pad paddle;

typedef struct 
{
    int exist;
    int x;
    int y;
    int dx;
    int dy;
    int radius;
    int r, g, b;
}dxball;

dxball ball;
dxball mainMenuRectangle[3];
dxball settingsBall[3];

typedef struct
{
    int exist;
    int x, y;
    double speed;
}p;

// handles all the powerups different states
p powerup[10];

typedef struct
{
    int8_t exist;
    int x;
    int y;
    int speed;
}fireBullet;

fireBullet bullet[10];

int sortedScore_idx[10]; // to store the index of the sorted scores
char highScoreName[10][20];// players name
int highScore[10]; // high scores
char playerName[20]; // to save the entered name from user
char scoretext[20]; // save score to show the score on the screen
char *button[] = {"sprites\\Dx-ball bg\\play.png", "sprites\\Dx-ball bg\\createlevel.png", "sprites\\Dx-ball bg\\settingsbutton.png", "sprites\\Dx-ball bg\\instructionbutton.png", "sprites\\Dx-ball bg\\highscorebutton.png", "sprites\\Dx-ball bg\\creditbutton.png", "sprites\\Dx-ball bg\\exitbutton.png"};

char *number[10] = {"sprites\\Dx-ball bg\\0.png", "sprites\\Dx-ball bg\\1.png", "sprites\\Dx-ball bg\\2.png", "sprites\\Dx-ball bg\\3.png", "sprites\\Dx-ball bg\\4.png", "sprites\\Dx-ball bg\\5.png", "sprites\\Dx-ball bg\\6.png", "sprites\\Dx-ball bg\\7.png", "sprites\\Dx-ball bg\\8.png", "sprites\\Dx-ball bg\\9.png"};

char *bg[] = {"sprites\\Dx-ball bg\\homepage.png", "sprites\\Dx-ball bg\\instruction.png", "sprites\\Dx-ball bg\\settings.png", "sprites\\Dx-ball bg\\credit2.png", "sprites\\Dx-ball bg\\gameover.png", "sprites\\Dx-ball bg\\pause.png", "sprites\\Dx-ball bg\\playbutton.png", "sprites\\Dx-ball bg\\name.png", "sprites\\Dx-ball bg\\highscore.png", "sprites\\Dx-ball bg\\theend.png", "sprites\\Dx-ball bg\\exit.png"};

char *brick[] = {"", "sprites\\Dx-ball bricks\\00_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\01_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\02_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\03_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\04_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\05_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\06_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\07_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\08_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\09_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\10_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\11_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\12_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\13_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\14_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\15_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\16_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\17_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\18_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\19_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\20_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\21_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\22_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\23_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\24_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\25_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\26_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\27_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\28_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\29_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\30_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball bricks\\31_PC Computer - DX-Ball - Bricks.png", "sprites\\Dx-ball power ups\\18_PC Computer - DX-Ball - Power-Ups.png"};

char *powerups[] = {"sprites\\Dx-ball power ups\\00_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\01_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\02_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\03_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\04_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\05_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\06_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\07_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\08_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\09_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\10_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\11_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\12_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\13_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\14_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\15_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\16_PC Computer - DX-Ball - Power-Ups.png", "sprites\\Dx-ball power ups\\17_PC Computer - DX-Ball - Power-Ups.png"};

char *levelups[] = {"sprites\\Dx-ball bg\\levelup1-Photoroom (0).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (1).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (2).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (3).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (4).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (5).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (6).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (7).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (8).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (9).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (10).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (11).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (12).png", "sprites\\Dx-ball bg\\levelup1-Photoroom (13).png"};


int8_t l[11][22][24] = {
    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//0  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//1  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//2  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//3  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//4  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//5  no use
        {0, 0,bb, 1,14, 1,14, 0, 0, 0, 0,bb, 8,12, 8, 0,bb,16,17,16,17, 0, 0, 0},//6
        {0, 0,bb,14, 1,14, 1, 0, 0, 0, 0,bb,12, 8,12, 0,bb,17,16,17,16, 0, 0, 0},//7
        {0, 0,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb, 0, 0, 0, 0,bb, 0, 0, 0, 0, 0, 0, 0},//8
        {0, 0,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb, 0, 0, 0, 0,bb, 0, 0, 0, 0, 0, 0, 0},//9
        {0, 0,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb, 0, 0, 0, 0,bb,17,16,17,16, 0, 0, 0},//10
        {0, 0,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb, 0, 0, 0, 0,bb,16,17,16,17, 0, 0, 0},//11
        {0, 0,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb, 0, 0, 0, 0,bb, 0, 0, 0, 0, 0, 0, 0},//12
        {0, 0,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb, 0, 0, 0, 0,bb, 0, 0, 0, 0, 0, 0, 0},//13
        {0, 0,bb,14, 1,14, 1, 0,12, 8,12,bb, 0, 0, 0, 0,bb,16,17,16,17, 0, 0, 0},//14
        {0, 0,bb, 1,14, 1,14, 0, 8,12, 8,bb, 0, 0, 0, 0,bb,17,16,17,16, 0, 0, 0},//15
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//16
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//17
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//18
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//19
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//20
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//21
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 1, 1,bb, 1, 1, 1,bb, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0,bb,16,16,16,bb,16,16,16,bb,16,16,16,bb,16,16,16, 0, 0, 0, 0}, 
        {0, 0, 0,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 0, 0, 0}, 
        {0, 0,bb,13,13,13,bb,13,13,13,bb,13,13,13,bb,13,13,13,bb,13,13,13, 0, 0}, 
        {0, 0, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 0, 0}, 
        {0, 0,16,16,bb,16,16,16,bb,16,16,16,bb,16,16,16,bb,16,16,16,bb,16, 0, 0}, 
        {0, 0, 1,bb, 1, 1, 1,bb, 1, 1, 1,bb, 1, 1, 1,bb, 1, 1, 1,bb, 1, 1, 0, 0}, 
        {0, 0, 0,16,16,16,bb,16,16,16,bb,16,16,16,bb,16,16,16,bb,16,16, 0, 0, 0}, 
        {0, 0, 0, 0, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 9,bb, 9, 9, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0,13,13,bb,13,13,13,bb,13,13,13,bb, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 9,bb,bb, 0, 0, 0, 0, 0, 0, 0, 0,bb,bb, 9,bb, 0, 0, 0, 0},
        {0, 0, 0,bb, 9,13, 9, 9,bb, 0, 0, 0, 0, 0, 0,bb, 9, 9,13, 9,bb, 0, 0, 0},
        {0, 0,bb,13,13,13,13,13, 9,bb, 0, 0, 0, 0,bb, 9,13,13,13,13,13,bb, 0, 0},
        {0, 0,12,12,12,12,12,bb,13, 9,bb, 0, 0,bb, 9,13,bb,12,12,12,12,12, 0, 0},
        {0, 0, 7, 7, 7, 7, 7,12,bb,13, 9,bb,bb, 9,13,bb,12, 7, 7, 7, 7, 7, 0, 0},
        {0, 0, 2, 2,bb,bb, 7, 7,12,bb,13, 9, 9,13,bb,12, 7, 7,bb,bb, 2, 2, 0, 0},
        {0, 0,bb,15, 2, 2,bb, 7, 7,12,bb,13,13,bb,12, 7, 7,bb, 2, 2,15,bb, 0, 0},
        {0, 0, 1,bb,15,15, 2,bb, 7, 7,12,bb,bb,12, 7, 7,bb, 2,15,15,bb, 1, 0, 0},
        {0, 0, 0, 1,bb,15,15, 2,bb, 7, 7,12,12, 7, 7,bb, 2,15,15,bb, 1, 0, 0, 0},
        {0, 0, 0, 0, 1,bb,15,15, 2,bb, 7, 7, 7, 7,bb, 2,15,15,bb, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1,bb,15,15, 2,bb, 7, 7,bb, 2,15,15,bb, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1,bb,15,15, 2,bb,bb, 2,15,15,bb, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1,bb,15,15, 2, 2,15,15,bb, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1,bb,15,15,15,15,bb, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1,bb, 1, 1,bb, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,bb,bb, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,12, 0, 0, 0, 0, 0,13,13,13,13, 0, 0, 0, 0, 0,12, 0, 0, 0, 0},
        {0, 0, 0, 0,12, 0, 0, 0, 0,13,13,13,13,13,13, 0, 0, 0, 0,12, 0, 0, 0, 0},
        {0, 0, 0, 0,12, 0, 0, 0,13,13,13,13,13,13,13,13, 0, 0, 0,12, 0, 0, 0, 0},
        {0, 0, 0, 0,12, 0, 0, 0, 0,13,13,13,13,13,13, 0, 0, 0, 0,12, 0, 0, 0, 0},
        {0, 0, 0, 0,12, 0, 0, 0, 0, 0,13,13,13,13, 0, 0, 0, 0, 0,12, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,12,12,12,12,14,14,14,14,14,14,12,12,12,12, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0, 0, 0, 0,14,15,15,15,15,14, 0, 0, 0, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0,bb,bb,bb,bb,bb,14,15, 2, 2,15,14,bb,bb,bb,bb,bb, 0, 0, 0, 0},
        {0, 0, 0, 0,bb,bb,bb,bb,bb,14,15, 2, 2,15,14,bb,bb,bb,bb,bb, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0, 0, 0, 0,14,15,15,15,15,14, 0, 0, 0, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 9, 9, 9, 9,14,14,14,14,14,14, 9, 9, 9, 9, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0, 0, 0,13,13,13,13, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0, 0,13,13,13,13,13,13, 0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0,13,13,13,13,13,13,13,13, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0, 0,13,13,13,13,13,13, 0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 0, 0, 0,13,13,13,13, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 2, 2,13,16,15,15,16,13, 2, 2, 2, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0, 2,13,13,16,15,15,15,15,16,13,13, 2, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2,13,16,15,15,14,14,15,15,16,13, 2, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0,13,16,15,15,14,17,17,14,15,15,16,13, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0,16,15,15,14,17, 0, 0,17,14,15,15,16, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0,15,15,14,17, 0, 2, 2, 0,17,14,15,15, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0,15,15,14,17, 0, 2, 2, 0,17,14,15,15, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0,16,15,15,14,17, 0, 0,17,14,15,15,16, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0,13,16,15,15,14,17,17,14,15,15,16,13, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2,13,16,15,15,14,14,15,15,16,13, 2, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0, 2,13,13,16,15,15,15,15,16,13,13, 2, 0,bb, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 2, 2,13,16,15,15,16,13, 2, 2, 2, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0,bb, 0, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 9, 9, 9, 7,bb, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,12,12,12,12, 9, 7,bb, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,bb,15,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,15,15,15,12,12,12, 9, 7,bb, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,14,15,bb,17,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,14,14,17,17,17,12,12,12, 9, 7,bb, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,14,bb,15,17,bb,17,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,14,15,15,15,17,17,17,12,12,12, 9, 7,bb, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,bb,14,15,bb,15,17,bb,17,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,14,14,14,15,15,15,17,17,17,12,12,12, 9, 7,bb, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,13,14,bb,14,15,bb,15,17,bb,17,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,13,13,14,14,14,15,15,15,17,17,17,12,12,12, 9, 7,bb, 0, 0, 0, 0, 0, 0},
        {0,13,bb,13,14,bb,14,15,bb,15,17,bb,17,12,bb,12, 9, 7, 0, 0, 0, 0, 0, 0},
        {0,13,13,13,13,14,14,14,15,15,15,17,17,17,12,12,12, 9, 7, 0, 0, 0, 0, 0},
        {0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 9, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,bb, 9, 0, 0, 0},
        {0, 9,bb, 9, 0, 0, 0, 9,bb, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0},
        {0, 0, 9, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 0, 0, 0, 7, 0,17, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0},
        {0, 7, 0,17, 0, 0, 0, 7,17,17, 0, 0, 0, 0, 0,17, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 7,17,17, 0, 0, 0, 7,17, 0, 0, 0, 0, 9, 0,17,17, 7, 0,17, 0, 0, 0, 0},
        {0, 7,17, 0, 9, 0, 0, 7, 0, 0, 9, 0, 9,bb, 9, 0,17, 7,17,17, 0, 0, 0, 0},
        {0, 7, 0, 9,bb, 9, 0, 7, 0, 9,bb, 9, 0, 9, 0, 0, 0, 7,17, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 9, 0, 0, 7, 0, 0, 9, 0,17, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 0, 7, 0, 7, 0, 0, 0, 7,17,17, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 7, 0,17, 0, 7, 0, 7, 0, 0,17, 7, 0,17, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 7, 0,17,17, 7, 0, 7, 0, 0, 0, 7, 0, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 7, 0, 0,17, 7, 0, 7, 0, 0, 0, 7, 0, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0,15,15,15,15, 0, 0,15,15,15,15,15,15,15,15,15,15,15,15,15,15, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//0  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//1  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//2  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//3  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//4  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//21 no use
        {0, 0, 0, 0,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb, 0, 0, 0, 0},//5  
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//6
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//7
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 7, 1, 1, 1, 7, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//8
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//9
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//10
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//11
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//12
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//13
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//14
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 1, 1, 1, 1, 1, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//15
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 7, 1, 1, 1, 7, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//16
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//17
        {0, 0, 0, 0,bb, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,bb, 0, 0, 0, 0},//18
        {0, 0, 0, 0,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb, 0, 0, 0, 0},//19
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//20
        
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//0 no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//1 no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//2 no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//3 no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//4 no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//5 no use
        {0, 0, 0, 1, 2, 3, 7, 8, 9,10,12,bb,bb,13,14,15,16,17,18,19,20, 0, 0, 0},//6 
        {0, 0, 1, 2, 3, 7, 7, 8, 9,10,12,13,14,15,16,17,18,19,18,20, 1, 2, 0, 0},//7
        {0, 1, 2, 3, 7, 8,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,bb,18,19,20, 1, 2, 0},//8 
        {0, 0, 1, 2, 3, 7, 7, 8, 9,10,12,13,14,15,16,17,18,19,18,20, 1, 2, 0, 0},//9 
        {0, 0, 0, 1, 2, 3, 7, 8, 9,10,12,bb,bb,13,14,15,16,17,18,19,20, 0, 0, 0},//10
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//11
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//12
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//13
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//14
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//15
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//16
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//17
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//18
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//19
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//20
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//21
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,14,bb,14,14,bb,14,14,bb,14,14,bb,14,14,bb,14,14,bb,14,14,bb,14,14, 0},
        {0,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,14, 0},
        {0,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,14, 0},
        {0,14, 0,13, 0,13, 0,13, 0,13, 0,13, 0,13, 0,13, 0,13, 0,13, 0,13,14, 0},
        {0,14,13, 0,13, 0,13, 0,13, 0,13,bb,13,bb,13, 0,13, 0,13, 0,13, 0,14, 0},
        {0,14, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17,14, 0},
        {0, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0,17, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,12,12,12,12,12,12,12,12,12,12, 0, 0,12,12,12,12,12,12,12,12,12,12, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,12, 0, 0,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,16, 0,16, 0, 0,16, 0,16, 0,12, 0, 0,12, 0,16, 0,16, 0, 0,16, 0,16, 0},
        {0,16, 0,16, 0, 0,16, 0,16, 0,12, 0, 0,12, 0,16, 0,16, 0, 0,16, 0,16, 0},
        {0,16,16,16,16,16,16,16,16, 0,12, 0, 0,12, 0,16,16,16,16,16,16,16,16, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },

    {
//col    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23  rows
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//0  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//1  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//2  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//3  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//4  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//5  no use
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//6
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//7
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//8
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//9
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//10
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//11
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//12
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//13
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//14
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//15
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//16
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//17
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//18
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//19
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//20
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}//21
    }
};

/*
    function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
    iClear();
    // main menu
    if (scene == -1)
    {
        lev = first_level;
        iSetColor(255, 210, 58);
        iShowBMP2(0, 0, bg[BG], 0);

        if(mainMenuRectangle[0].exist)
        {
            iShowBMP2(mainMenuRectangle[0].x, mainMenuRectangle[0].y, button[mainMenuRectangle[0].dx], 0);
        }
    }
    // new level start screen
    else if(scene == 6)
    {
        iShowBMP2(0, 0, levelups[lev], 0);
    }
    // game levels
    else if (scene == 0)
    {
        paddle.position = screenwidth / 2 - paddle.dx / 2 + paddle.x;
        iSetColor(2, 5, 5);
        iFilledRectangle(0, 0, screenwidth, screenheight);

        if(fire == 1)
        {
            showFire();
        }

        iSetColor(12, 255, 250);
        iFilledCircle(paddle.position, paddle.dy / 2 + paddle.y, paddle.dy / 2);
        iFilledCircle(paddle.position + paddle.dx, paddle.y + paddle.dy / 2, paddle.dy / 2);

        iSetColor(99, 99, 99);
        iFilledRectangle(paddle.position, paddle.y, paddle.dx, paddle.dy);

        iSetColor(0, 0, 0);
        iLine(paddle.position, paddle.y + paddle.dy * 2 / 3, paddle.position + paddle.dx, paddle.y + paddle.dy * 2 / 3);
        iLine(paddle.position, paddle.y + paddle.dy / 3, paddle.position + paddle.dx, paddle.y + paddle.dy / 3);

        showBall();

        if(playBall == 0) 
        {
            ball.x = paddle.position + paddle.dx / 2;
        }

        showBrick();
        showPowerups();

        iSetColor(235, 235, 235);
        sprintf(scoretext, "SCORE : %d", score);
        iText(825, screenheight - 25, "LIVES : ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(20, screenheight - 25, scoretext, GLUT_BITMAP_TIMES_ROMAN_24);

        // showing lives on the top right
        for(int i = 0; i < lives; i++)
        {
            iFilledCircle(i * 18 + 100 + 825, screenheight - 15, 7);
        }

        if(isResumed == 0)
        {
            iShowBMP2(screenwidth / 2 - 455 / 2, screenheight / 2 - 455 / 2, bg[pause], 0);
            iCircle(527 - 30, (364 + 391) / 2, 15, 1000);
            iCircle(527 - 30, (316 + 347) / 2, 15, 1000);
            iFilledCircle(settingsBall[2].x, settingsBall[2].y, 10, 1000);
        }
    }
    // create levels
    else if(scene == 1)
    {
        iSetColor(255, 255, 255);

        for(int i = 6; i < 22; i++)
        {
            for(int j = 1; j < 24; j++)
            {
                if(l[createlevelnum][i][j] > 0)
                {
                    iShowBMP2(-14 + j * 47, 250 + (21 - i) * 24, brick[l[createlevelnum][i][j]], 0);
                }
            }
        }

        iFilledRectangle(mainMenuRectangle[1].x, mainMenuRectangle[1].y, mainMenuRectangle[1].dx, mainMenuRectangle[1].dy);
        showgrid();
        showBrickOptions();

        iText(30, 223, "Left click on a brick to select it. Left click and drag across the grid to draw. Right click and drag across the grid to erase. Press c to clear all.", GLUT_BITMAP_HELVETICA_12);

        iShowBMP2(930, 20, bg[playbutton], 0);
    }
    // settings
    else if(scene == 2)
    {
        iSetColor(255, 255, 255);

        iShowBMP2(0, 0, bg[settings], 0);
        iFilledCircle(settingsBall[0].x, settingsBall[0].y, 10, 1000);
        iFilledCircle(settingsBall[1].x, settingsBall[1].y, 10, 1000);

        iCircle(490, (539 + 576) / 2, 15, 1000);
        iCircle(490, (512 + 476) / 2, 15, 1000);
        iCircle(490, (413 + 452) / 2, 15, 1000);
        iCircle(480, (154 + 196) / 2, 15, 1000);
        iCircle(480, (271 + 227) / 2, 15, 1000);
    }
    // instructions
    else if(scene == 3)
    {
        iShowBMP2(0, 0, bg[instructions], 0);
    }
    // credits
    else if (scene == 5)
    {
        iShowBMP2(0, 0, bg[credit], 0);
    }
    // gameover
    else if(scene == 7)
    {
        lev = first_level;
        iShowBMP2(0, 0, bg[gameover], 0);
        int k = 0;
        int scorenumber[10];
        int score2 = score;

        while(score > 0)
        {
            scorenumber[k++] = score % 10;
            score /= 10;
        }

        score = score2;
        for(int i = k - 1; i >= 0; i--)
        {
            iShowBMP2(427 + (k - i - 1) * 52, 330, number[(int)scorenumber[i]], 0);
        }
    }
    // enter name
    else if(scene == 8)
    {
        iShowBMP2(0, 0, bg[entername], 0);
        iSetColor(255, 255, 255);
        iRectangle(455, 357, 441, 40);
        iText(465, 362 + 7, playerName, GLUT_BITMAP_TIMES_ROMAN_24);
    }
    // highscore
    else if(scene == 4)
    {
        iSetColor(255, 255, 255);
        iShowBMP2(0, 0, bg[highscore], 0);

        for(int i = 0; i < 10; i++)
        {
            char ch[20];
            sprintf(ch, "%d", highScore[i]);

            iText(300, 550 - i * 50, highScoreName[i], GLUT_BITMAP_TIMES_ROMAN_24);
            iText(695, 550 - i * 50, ch, GLUT_BITMAP_TIMES_ROMAN_24);

            iLine(292, 550 - i * 50 - 25, 750, 550 - i * 50 - 25);
        }
    }
    // end of game
    else if(scene == 9)
    {
        iShowBMP2(0, 0, bg[theend], 0);
        lev = first_level;
    }
    // exit
    else if(scene == 10)
    {
        iShowBMP2(0, 0, bg[Exit], 0);
        waiting++;
        if(waiting >= 120)
            exit(0);
    }
}

/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
    if(scene == 1 && leftclick)
    {
        int j = (mx + 14) / 47, i = 21 - (my - 250) / 24;

        if(i < 22 && j < 23 && i > 5 && j > 0)
        {
            l[createlevelnum][i][j] = selectedBrick;
        }
    }

    else if(scene == 1 && !leftclick)
    {
        int j = (mx + 14) / 47, i = 21 - (my - 250) / 24;

        if(i < 22 && j < 23 && i > 5 && j > 0)
        {
            if(l[createlevelnum][i][j] > 0)
            {
                l[createlevelnum][i][j] = 0;
            }
        }
    }
}
/*
    called when mouse is moved.
    mx and my is the position of the mouse pointer
*/ 
void iPassiveMouseMove(int mx, int my)
{
    if(scene == 0 && isResumed)
    {
        if(screenwidth / 2 - paddle.dx / 2 + (mx - screenwidth / 2) * 1.9 - paddle.dy / 3 > 0 && screenwidth / 2 - paddle.dx / 2 + paddle.dx + (mx - screenwidth / 2) * 1.9 + paddle.dy / 3 < screenwidth)
            paddle.x = (mx - screenwidth / 2) * 1.9; // to make it harder to control using mouse
    }

    if (scene == -1)
    {
        // scene = 0;
        if(mx > 505 && mx < 595 && my > 422 && my < 459)
        {
            mainMenuRectangle[0].exist = 1;
            mainMenuRectangle[0].x = 505 - 5;
            mainMenuRectangle[0].y = 422 - 12;
            mainMenuRectangle[0].dx = 0;
        }

        else if(mx > 419 && mx < 682 && my > 362 && my < 391)
        {
            mainMenuRectangle[0].exist = 1;
            mainMenuRectangle[0].x = 419 - 5;
            mainMenuRectangle[0].y = 362 - 12;
            mainMenuRectangle[0].dx = 1;
        }

        else if(mx > 459 && mx < 637 && my > 300 && my < 330)
        {
            mainMenuRectangle[0].exist = 1;
            mainMenuRectangle[0].x = 459 - 5;
            mainMenuRectangle[0].y = 300 - 12;
            mainMenuRectangle[0].dx = 2;
        }

        else if(mx > 427 && mx < 673 && my > 239 && my < 267)
        {
            mainMenuRectangle[0].exist = 1;
            mainMenuRectangle[0].x = 427 - 5;
            mainMenuRectangle[0].y = 239 - 12;
            mainMenuRectangle[0].dx = 3;
        }

        else if(mx > 440 && mx < 659 && my > 180 && my < 206)
        {
            mainMenuRectangle[0].exist = 1;
            mainMenuRectangle[0].x = 440 - 5;
            mainMenuRectangle[0].y = 180 - 12;
            mainMenuRectangle[0].dx = 4;
        }

        else if(mx > 484 && mx < 618 && my > 118 && my < 146)
        {
            mainMenuRectangle[0].exist = 1;
            mainMenuRectangle[0].x = 484 - 5;
            mainMenuRectangle[0].y = 118 - 12;
            mainMenuRectangle[0].dx = 5;
        }

        else if(mx > 509 && mx < 590 && my > 56 && my < 84)
        {
            mainMenuRectangle[0].exist = 1; 
            mainMenuRectangle[0].x = 509 - 5;
            mainMenuRectangle[0].y = 56 - 12;
            mainMenuRectangle[0].dx = 6;
        }

        else mainMenuRectangle[0].exist = 0;
    }
}

/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // play
        if(scene == 0) 
        {
            playBall = 1;

            if(isResumed == 0)
            {
                if(mx > 527 && mx < 585 && my > 364 && my < 391)
                {
                    // sound on
                    music = 1;
                    settingsBall[2].x = 527 - 30;
                    settingsBall[2].y = (364 + 391) / 2;
                }

                else if(mx > 527 && mx < 606 && my > 316 && my < 347)
                {
                    // sound off
                    music = 0;
                    settingsBall[1].x = 480;
                    settingsBall[1].y = (196 - 154) / 2 + 154;
                    settingsBall[2].x = 527 - 30;
                    settingsBall[2].y = (316 + 347) / 2;
                }

                else if(mx > 450 && mx < 637 && my > 474 && my < 506)
                {
                    if(music)
                        PlaySound("sprites\\Dx-ball sounds\\minecraft_memories.wav", NULL, SND_LOOP | SND_ASYNC);
                    isResumed = 1;
                }

                else if(mx > 409 && mx < 675 && my > 258 && my < 289)
                {
                    playerName_idx = 0;
                    if(music)
                    {
                        PlaySound("sprites\\Dx-ball sounds\\the_fat.wav", NULL, SND_LOOP | SND_ASYNC);
                    }
                    isResumed = 1;
                    lives = livecount;
                    scene = -1;
                }

            }

            passiveMouseControl = mx;
            mouseClicked = 1;
        }
        // main menu
        else if (scene == -1)
        {
            // scene = 0;
            if(mx > 505 && mx < 595 && my > 422 && my < 459)
            {
                playerName[0] = '\0';

                for(int i = 6; i < 22; i++)
                {
                    for(int j = 1; j < 24; j++)
                    {
                        if(l[0][i][j] < 0)
                        {
                            l[0][i][j] *= -1;
                        }
                    }
                }   

                scene = 8; // enter name
            }

            if(mx > 419 && mx < 682 && my > 362 && my < 391)
            {
                if(music)
                    PlaySound("sprites\\Dx-ball sounds\\minecraft_memories.wav", NULL, SND_LOOP | SND_ASYNC);

                scene = 1; // create levels
            }

            if(mx > 459 && mx < 637 && my > 300 && my < 330)
            {
                scene = 2; // settings
            }

            if(mx > 427 && mx < 673 && my > 239 && my < 267)
            {
                scene = 3; // instructions
            }

            if(mx > 440 && mx < 653 && my > 180 && my < 206)
            {
                scene = 4; // highscores
            }

            if(mx > 484 && mx < 618 && my > 118 && my < 146)
            {
                scene = 5; // credits
            }

            if(mx > 509 && mx < 590 && my > 56 && my < 84)
            {
                PlaySound(0, 0, 0);
                scene = 10;
            }
        }

        else if(scene == 6)
        {
            if(mx > 856 && mx < 1014 && my > 73 && my < 99)
            {
                if(music)
                    PlaySound("sprites\\Dx-ball sounds\\minecraft_memories.wav", NULL, SND_LOOP | SND_ASYNC);

                scene = 0; // start game
            }
        }

        else if(scene == 3)
        {
            if(mx > 30 && mx < 176 && my > 42 && my < 72)
            {
                scene = -1; // main menu
            }
        }
        // settings
        else if(scene == 2)
        {
            if(mx > 525 && mx < 656 && my > 539 && my < 576)
            {
                // Easy
                velocity = 11;
                settingsBall[0].x = 490;
                settingsBall[0].y = (576 - 539) / 2 + 539;
            }

            if(mx > 519 && mx < 732 && my > 476 && my < 512)
            {
                // Medium
                velocity = 13;
                settingsBall[0].x = 490;
                settingsBall[0].y = (512 - 476) / 2 + 476;
            }

            if(mx > 522 && mx < 689 && my > 413 && my < 452)
            {
                // Hard
                velocity = 15;
                settingsBall[0].x = 490;
                settingsBall[0].y = (452 - 413) / 2 + 413;
            }

            if(mx > 510 && mx < 590 && my > 227 && my < 271)
            {
                // On
                PlaySound("sprites\\Dx-ball sounds\\the_fat.wav", NULL, SND_LOOP | SND_ASYNC);

                music = 1;
                settingsBall[1].x = 480;
                settingsBall[1].y = (271 - 227) / 2 + 227;
            }

            if(mx > 510 && mx < 610 && my > 154 && my < 196)
            {
                // Off
                music = 0;
                PlaySound(0, 0, 0);

                settingsBall[2].x = 527 - 30;
                settingsBall[2].y = (316 + 347) / 2;
                settingsBall[1].x = 480;
                settingsBall[1].y = (196 - 154) / 2 + 154;
            }

            if(mx > 49 && mx < 229 && my > 43 && my < 78)
            {
                scene = -1;
            }
        }
        // credits
        else if(scene == 5)
        {
            if(mx > 47 && mx < 211 && my > 58 && my < 89)
            {
                scene = -1;
            }
        }

        else if(scene == 7)
        {
            if(mx > 672 && mx < 1060 && my > 55 && my < 76)
            {
                if(music)
                    PlaySound("sprites\\Dx-ball sounds\\the_fat.wav", NULL, SND_LOOP | SND_ASYNC);

                score = 0;
                scene = -1;
            }
        }
        // create levels
        else if(scene == 1)
        {
            leftclick = 1;
            if (mx > 930 && mx < 930 + 134 && my > 20 && my < 34 + 20)
            {
                lev = 10;
                scene = 0;
            }
            
            // first row bricks
            if(mx > 33 && mx < 76 && my > 147 && my < 168)
            {
                selectedBrick = 12;

                mainMenuRectangle[1].x = 33 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 76 - 33 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 196 && mx < 241 && my > 147 && my < 168)
            {
                selectedBrick = 13;

                mainMenuRectangle[1].x = 196 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 241 - 196 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 362 && mx < 407 && my > 147 && my < 168)
            {
                selectedBrick = 14;

                mainMenuRectangle[1].x = 362 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 407 - 362 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 527 && mx < 571 && my > 147 && my < 168)
            {
                selectedBrick = 15;

                mainMenuRectangle[1].x = 527 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 571 - 527 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 691 && mx < 735 && my > 147 && my < 168)
            {
                selectedBrick = 16;

                mainMenuRectangle[1].x = 691 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 735 - 691 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 855 && mx < 899 && my > 147 && my < 168)
            {
                selectedBrick = 17;

                mainMenuRectangle[1].x = 855 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 899 - 855 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 1021 && mx < 1065 && my > 147 && my < 168)
            {
                selectedBrick = 22;

                mainMenuRectangle[1].x = 1021 - 10;
                mainMenuRectangle[1].y = 147 - 10;
                mainMenuRectangle[1].dx = 1065 - 1021 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            // second row bricks
            if(mx > 33 && mx < 76 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 1;

                mainMenuRectangle[1].x = 33 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 76 - 33 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 196 && mx < 241 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 2;

                mainMenuRectangle[1].x = 196 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 241 - 196 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 362 && mx < 407 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 3;

                mainMenuRectangle[1].x = 362 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 407 - 362 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 527 && mx < 571 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 6;

                mainMenuRectangle[1].x = 527 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 571 - 527 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 691 && mx < 735 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 7;

                mainMenuRectangle[1].x = 691 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 735 - 691 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 855 && mx < 899 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 9;

                mainMenuRectangle[1].x = 855 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 899 - 855 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }

            if(mx > 1021 && mx < 1065 && my > 147 - 50 && my < 168 - 50)
            {
                selectedBrick = 10;

                mainMenuRectangle[1].x = 1021 - 10;
                mainMenuRectangle[1].y = 147 - 10 - 50;
                mainMenuRectangle[1].dx = 1065 - 1021 + 20;
                mainMenuRectangle[1].dy = 168 - 147 + 20;
            }
        }
        // enter name
        else if(scene == 8)
        {
            if(mx > 893 && mx < 1060 && my > 39 && my < 68)
            {
                PlaySound(0, 0, 0);
                // storing only the first name
                for(int i = 0; playerName[i] != '\0'; i++)
                {
                    if(playerName[i] == ' ')
                    {
                        playerName[i] = '\0';
                        break;
                    }
                }

                scene = 6;
            }
        }
        // highscores
        else if(scene == 4)
        {
            if(mx > 36 && mx < 180 && my > 41 && my < 71)
            {
                scene = -1;
            }
        }
        // end of game
        else if(scene == 9)
        {
            if(mx > 592 && mx < 1066 && my > 40 && my < 64)
            {
                if(music)
                    PlaySound("sprites\\Dx-ball sounds\\the_fat.wav", NULL, SND_LOOP | SND_ASYNC);

                scene = -1;
            }
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
        if(scene == 1)
        {
            leftclick = 0;
        }
    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(scene == 0)
    {
        if(key == 27)
        {
            PlaySound(0, 0, 0);
            isResumed = 0;
        }
    }

    if (key == 'q')
    {
        // exit(0);
    }

    if (key == 'p')
    {
        iPauseTimer(0);
    }

    if(key == 'r')
    {
        iResumeTimer(0);
    }

    if (key == 'a')
    {
        if(paddle.position - paddle.dy / 2 - 30 <= 0)
            paddle.x -= paddle.position - paddle.dy / 2;
        else
            paddle.x -= 30;
    }

    if (key == 'd')
    {
        if ((paddle.position + paddle.dx + paddle.dy / 2 + 30) >= screenwidth)
            paddle.x += screenwidth - paddle.position - paddle.dx - paddle.dy / 2;
        else 
            paddle.x += 30;
    }

    if (key == ' ')
    {
        if(fire == 1)
        {
            spacePressed++;

            if(spacePressed % 8 == 1)
            {
                bullet[bullet_idx].exist = 1;
                bullet[bullet_idx].x = paddle.position;
                bullet[bullet_idx].y = paddle.y + paddle.dy;
                bullet_idx++;
                if(bullet_idx >= 10) bullet_idx = 0;
            }
        }
    }

    if(scene == 1)
    {
        if(key == 'c')
        {
            for(int i = 6; i < 22; i++)
                for(int j = 1; j < 24; j++)
                {
                    if(l[createlevelnum][i][j] > 0)
                    {
                        l[createlevelnum][i][j] = 0;
                    }
                }
        }
    }

    if(scene == 8)
    {
        if(key != '\b')
        {
            playerName[playerName_idx] = key;
            playerName_idx++;
            playerName[playerName_idx] = '\0';
        }

        else 
        {
            if(playerName_idx > 0)
            {
                playerName_idx--;
                playerName[playerName_idx] = '\0';
            }
        }
    }

    // place your codes for other keys here
}

/*
    function iSpecialKeyboard() is called whenver user hits special keys like-
    function keys, home, end, pg up, pg down, arraows etc. you have to use
    appropriate constants to detect them. A list is:
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
    GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
    GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
    */
void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_INSERT)
    {

    }

    if (key == GLUT_KEY_RIGHT)
    {

    }

    if (key == GLUT_KEY_LEFT)
    {

    }

    if (key == GLUT_KEY_UP)
    {

    }

    if (key == GLUT_KEY_DOWN)
    {

    }
    // place your codes for other keys here
}
// shows bullets and check if it collide with bricks , then explodes them
void showFire()
{
    iSetColor(255, 253, 85);
    // drawing the gun pointer
    iFilledEllipse(paddle.position, paddle.y + paddle.dy, 4, 14);
    iFilledEllipse(paddle.position + paddle.dx, paddle.y + paddle.dy, 4, 14);

    for(int i = 0; i < 10; i++)
    {
        if(bullet[i].exist == 1)
        {
            iSetColor(255, 0, 0);
            // drawing the bullet
            iFilledEllipse(bullet[i].x, bullet[i].y, 3, 8);
            iFilledEllipse(bullet[i].x + paddle.dx, bullet[i].y, 3, 8);
            // left side bullet collision
            if(l[lev][21 - (bullet[i].y - 250) / 24][(bullet[i].x + 14) / 47] > 0)
            {    
                explodeBrick(21 - (bullet[i].y - 250) / 24, (bullet[i].x + 14) / 47);
                bullet[i].exist = 0;
            }
            // right side bullet collision
            if(l[lev][21 - (bullet[i].y - 250) / 24][(bullet[i].x + paddle.dx + 14) / 47] > 0)
            {    
                explodeBrick(21 - (bullet[i].y - 250) / 24, (bullet[i].x + paddle.dx + 14) / 47);
                bullet[i].exist = 0;
            }

            if(bullet[i].y >= screenheight)
            {
                bullet[i].exist = 0;
            }
            // updating the bullet position
            bullet[i].y += 5;
        }
            
    }
}

void showgrid()
{
    iSetColor(255, 255, 255);

    for(int i = 0; i < 23; i++)
    {
        iLine(33 + i * 47, 250, 33 + i * 47, 250 + 24 * 16);
    }

    for(int i = 0; i < 17; i++)
    {
        iLine(33, 250 + (16 - i) * 24, screenwidth - 33, 250 + (16 - i) * 24);
    }
}

void showBrickOptions()
{
    int k = 1;

    for(int j = 0; j < 2; j++)
        for (int i = 0; i < 7; i++)
        {
            iShowBMP2(33 + i * 164.5, 94 + j * 50, brick[k], 0);

            switch (k)
            {
                case 3:
                    k++;
                case 4:
                    k++;
                case 5:
                    k++;
                    break;

                case 7:
                    k++;
                case 8:
                    k++;
                    break;

                case 10:
                    k++;
                case 11:
                    k++;
                    break;

                case 17:
                    k++;
                case 18:
                    k++;
                case 19:
                    k++;
                case 20:
                    k++;
                case 21:
                    k++;
                    break;

                default:
                    k++;
                    break;
            }
        }
}

void showBall()
{
    iSetColor(ball.r, ball.g, ball.b);
    iFilledCircle(ball.x, ball.y, ball.radius, 1000);
}

void showBrick()
{
    ex = 0;

    for(int i = 6; i < 22; i++)
        for(int j = 1; j < 24; j++)
        {
            if(l[lev][i][j] > 0)
            {
                if(l[lev][i][j] != 6) ex = 1;
                iShowBMP2(-14 + j * 47, 250 + (21 - i) * 24, brick[l[lev][i][j]], 0);
            }
        }
    // check if all the bricks are destroyed. if so then next level
    if(ex == 0) 
    {
        // reinitializing the prev. level bricks
        for(int i = 6; i < 22; i++)
            for(int j = 1; j < 24; j++)
            {
                if(l[lev][i][j] < 0)
                {
                    l[lev][i][j] *= -1;
                }
            }

        PlaySound(0, 0, 0);
        lev++;
        powerup_idx = -1;
        // reinitializing the next level bricks
        for(int i = 6; i < 22; i++)
            for(int j = 1; j < 24; j++)
            {
                if(l[lev][i][j] < 0)
                {
                    l[lev][i][j] *= -1;
                }
            }

        sleep(2);

        if(lev == numoflevel + 1)
        {
            if(music)
                PlaySound("sprites\\Dx-ball sounds\\gta_sanandreas.wav", NULL, SND_LOOP | SND_ASYNC);

            scene = 9;
        }

        else if(lev == 11)
        {
            scene = -1;
        }

        else 
            scene = 6; // showing level number to start

        playBall = 0;
        resetEverything();
    }
}

double ballBounceX()
{
    double ans = ball.dx;
    int i = 21 - (ball.y - 250) / 24;
    int j = (ball.x + 14) / 47;
    int y = i;

    if (ball.x >= screenwidth || ball.x <= 0)
    {
        ans = -ball.dx;
    }

    if ((ball.x > paddle.position - paddle.dy / 2 - ball.radius && ball.x < paddle.position + paddle.dx + paddle.dy / 2 + ball.radius) && (ball.y <= paddle.y + paddle.dy && ball.y > paddle.y + paddle.dy / 2))
    {
        paddle_bounce = 1;
        ans = velocity * cos(theta());
    }

    return round(ans);
}

double ballBounceY()
{
    double ans = ball.dy;
    int i = 21 - (ball.y - 250) / 24;
    int j = (ball.x + 14) / 47;
    int x = j;

    if(ball.y < 0) 
    {
        playBall = 0;
        lives--;
        sleep(1);

        if(lives == 0)
        {
            if(music)
                PlaySound("sprites\\Dx-ball sounds\\sangsad_tv_bangladesh.wav", NULL, SND_FILENAME | SND_ASYNC);

            powerup_idx = -1;
            playerName_idx = 0;
            lev = 0;

            if(score > lowestscore) 
            {
                highScore[lowestscoer_idx] = score;
                strcpy(highScoreName[lowestscoer_idx], playerName);
                sortScore();
                storeScore();
            }

            lives = livecount;
            scene = 7;
        }

        resetEverything();
        resetEverything();
    }

    if (ball.y >= screenheight)
    {    
        ans = -ball.dy;
    }// yes

    if ((ball.x > paddle.position - paddle.dy / 2 && ball.x < paddle.position + paddle.dx + paddle.dy / 2) && (ball.y <= paddle.y + paddle.dy && ball.y > paddle.y + paddle.dy / 2))
    {
        ans = velocity * sin(theta());
    }

    return round(ans);
}

void powerActivate(int i)
{
    if(i == 0)
    {
        ball.radius = 10;
    }

    else if(i == 1)
    {
        lives++;
    }

    else if(i == 2)
    {
        bigBall = 1;
        velocity += 5;
    }

    else if(i == 3)
    {
        sleep(1);
        playBall = 0;
        lives--;

        if (lives == 0)
        {
            if(music)
                PlaySound("sprites\\Dx-ball sounds\\sangsad_tv_bangladesh.wav", NULL, SND_FILENAME | SND_ASYNC);

            powerup_idx = -1;
            playerName_idx = 0;
            lev = 0;

            if(score >= lowestscore)
            {
                highScore[lowestscoer_idx] = score;
                strcpy(highScoreName[lowestscoer_idx], playerName);

                sortScore();
                storeScore();
            }

            lives = livecount;
            scene = 7;
        }

        resetEverything();
    }
    
    else if(i == 4)
    {
        
    }

    else if(i == 5)
    {
        paddle.dx = 100;
    }

    else if(i == 6)
    {
        fire = 1;
    }

    else if(i == 7)
    {
        throBall = 1;
        ball.r = 255, ball.g = 14, ball.b = 52;
    }

    else if(i == 8)
    {
        paddle.dx = 250;
    }

    else if(i == 9)
    {
        fireBall = 1;
        ball.r = 255, ball.g = 122, ball.b = 25;
    }
}
// shows moving powerups across the screen and check if it collide with paddle
void showPowerups()
{ 
    for(int i = 0; i < 10; i++)
    {
        if(powerup[i].exist && isResumed)
        {
            iShowBMP2(powerup[i].x, powerup[i].y , powerups[i], 0);

            fps *= 0.995;
            powerup[i].y -= powerup[i].speed;

            if(powerup[i].y <= 0)
                powerup[i].exist = 0;

            if(((powerup[i].x > paddle.position - paddle.dy / 2 - 33 && powerup[i].x < paddle.position + paddle.dx + paddle.dy / 2) && (powerup[i].y <= paddle.y + paddle.dy - 2)))
            {    
                powerup[i].exist = 0;
                powerActivate(i);
            }
        }
    }

}
// explodes bricks around it
void explodeBrickAround(int i, int j)
{
    l[lev][i][j] *= -1;
    score += 10;
    // Iterate over the 3x3 grid around the brick
    for (int a = -1; a <= 1; a++) 
    {
        for (int b = -1; b <= 1; b++) 
        {
            if (a == 0 && b == 0) continue; // Skip the center brick itself
            // Recursively process neighbors
            if (l[lev][i + a][j + b] > 21 && l[lev][i + a][j + b] < 27) 
            {
                explodeBrick(i + a, j + b);
            }
            // Mark neighboring bricks as destroyed
            if (l[lev][i + a][j + b] > 0) 
            {
                score += 10;
                l[lev][i + a][j + b] *= -1;
            }
            
        }
    }
}
// plays brick exploding animation
void explodeBrick(int i, int j)
{
    if (l[lev][i][j] <= 0 || l[lev][i][j] == 6)
        return;
    // Check if the current brick is explosive
    if (l[lev][i][j] > 21 && l[lev][i][j] < 27) {
        // Destroy the current brick
        explodeBrickAround(i, j);
    } 

    else if((l[lev][i][j] > 2 && l[lev][i][j] < 5) && (ball.radius != 10) && (!fire)) 
    {
        score += 10;
        l[lev][i][j]++;
    }

    else 
    {
        score += 10;
        // If the brick is not explosive, just destroy it
        l[lev][i][j] *= -1;
    }
}

void setPowerupPosition(int i, int j)
{
    // check if the ball is within the range of the matrix
    if(i < 22 && j < 24)
    {
        if(l[lev][i][j] != 6) powerupIncomingCnt++;

        if(powerupIncomingCnt % 6 == 1)
        {
            powerup_idx++;

            if(powerup_idx == 4) powerup_idx++;

            if(powerup_idx < 10)
            {
                powerup[powerup_idx].exist = 1;
                powerup[powerup_idx].x = ball.x;
                powerup[powerup_idx].y = ball.y;
                powerup[powerup_idx].speed = 2;
            }

            else powerup_idx = -1;
        }

        explodeBrick(i, j);

        if(!throBall)
        {
            if(fireBall)
            {
                l[lev][i][j] *= -1;
                explodeBrickAround(i, j);
            }
            // working one
            if(ball.x >  - 14 + j * 47 - ball.radius && ball.x < -14 + j * 47 + 47 + ball.radius)
            {
                ball.dy = -ball.dy;
            }

            else
            {
                ball.dx = - ball.dx;
            }
        }

        else
        {
            if(fireBall)
            {
                l[lev][i][j] *= -1;
                explodeBrickAround(i, j);
            }
        }
    }   
}
// ball bounces when it collides with the brick
void brickBounce()
{
    int i, j;
    // working one
    if (l[lev][21 - (ball.y + ball.radius - 250) / 24][(ball.x + ball.radius + 14) / 47] > 0)
    {
        i = 21 - (ball.y + ball.radius - 250) / 24;
        j = (ball.x + ball.radius + 14) / 47;
        setPowerupPosition(i, j);
    }  

    else if(l[lev][21 - (ball.y + ball.radius - 250) / 24][(ball.x - ball.radius + 14) / 47] > 0)
    {
        i = 21 - (ball.y + ball.radius - 250) / 24;
        j = (ball.x - ball.radius + 14) / 47;
        setPowerupPosition(i, j);
    }

    else if(l[lev][21 - (ball.y - ball.radius - 250) / 24][(ball.x - ball.radius + 14) / 47] > 0)
    {
        i = 21 - (ball.y - ball.radius - 250) / 24;
        j = (ball.x - ball.radius + 14) / 47;
        setPowerupPosition(i, j);
    }

    else if(l[lev][21 - (ball.y - ball.radius - 250) / 24][(ball.x + ball.radius + 14) / 47] > 0)
    {
        i = 21 - (ball.y - ball.radius - 250) / 24;
        j = (ball.x + ball.radius + 14) / 47;
        setPowerupPosition(i, j);
    }
}
// angle of the projection of the ball when collides 
double theta()
{
    double x = (float)(paddle.position + paddle.dx + paddle.dy / 2 - ball.x);
    double m = (float)120 / (paddle.dx + paddle.dy);
    double c = 30;
    double ans = m * x + c;
    return PI * ans / 180;
}
// bombing brick sprite change
void bbchange()
{
    
    for(int j = 6; j < 22; j++)
        for(int k = 1; k < 24; k++)
        {
            if(isResumed && l[lev][j][k] > 21 && l[lev][j][k] < 27)
            {
                l[lev][j][k]++;
            }
            if(isResumed && l[lev][j][k] > 26)
            {
                l[lev][j][k] = 22;
            }
        }
}
// ball reflection
void change()
{
    if(playBall && isResumed)
    {
        ball.dx = (int)ballBounceX();
        ball.dy = (int)ballBounceY();
        brickBounce();
        ball.x += ball.dx;
        ball.y += ball.dy;
    }
}

void resetEverything()
{
    // scene = 6;
    throBall = 0;
    fireBall = 0;
    fire = 0;
    fps = 1.2;
    powerupIncomingCnt = 0;

    paddle.dx = 150;
    paddle.dy = 20;
    paddle.x = 0;
    paddle.y = 10;
    paddle.position = screenwidth / 2 - paddle.dx / 2 + paddle.x;

    ball.radius = 7;
    ball.r = 235, ball.g = 235, ball.b = 235;
    ball.dx = velocity * cos(initialBallAngle * PI / 180);
    ball.dy = velocity * sin(initialBallAngle * PI / 180);
    ball.y = paddle.y + paddle.dy + ball.radius;
    ball.x = paddle.position + paddle.dx / 2;

    velocity = 11;

    for(int i = 0; i < 10; i++)
    {
        powerup[i].exist = 0;
        powerup[i].speed = 2;
        bullet[i].exist = 0;
        bullet[i].speed = 5;
    }
}

void storeScore()
{
    FILE *filewrite = fopen("highscore.txt", "w");

    for(int i = 0; i < 10; i++)
    {
        fprintf(filewrite, "%s\t%d\n", highScoreName[i], highScore[i]);
    }

    fclose(filewrite);
}

void sortScore()
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = i + 1; j < 10; j++)
        {
            if(highScore[i] < highScore[j])
            {
                int temp1 = highScore[i];
                highScore[i] = highScore[j];
                highScore[j] = temp1;

                char temp2[20];
                strcpy(temp2, highScoreName[i]);
                strcpy(highScoreName[i], highScoreName[j]);
                strcpy(highScoreName[j], temp2);
            }
        }
    }
}

void r_and_w_highscore()
{
    FILE *fileread = fopen("highscore.txt", "r");

    for(int i = 0; !feof(fileread); i++)
    {  
        fscanf(fileread, "%s\t%d", highScoreName[i], &highScore[i]);
    }

    fclose(fileread);

    for(int i = 0; i < 10; i++)
    {
        if(highScore[i] < lowestscore) 
        {
            lowestscoer_idx = i;
            lowestscore = highScore[i];
        }
    }

    sortScore();
    storeScore();
}

int main()
{
    settingsBall[0].x = 490;
    settingsBall[0].y = (576 - 539) / 2 + 539;
    settingsBall[1].x = 480;
    settingsBall[1].y = (271 - 227) / 2 + 227;
    settingsBall[2].x = 527 - 30;
    settingsBall[2].y = (391 + 364) / 2;  

    if(music)
        PlaySound("sprites\\Dx-ball sounds\\the_fat.wav", NULL, SND_LOOP | SND_ASYNC);

    r_and_w_highscore();
    resetEverything();

    iSetTimer(5, change);
    iSetTimer(50, bbchange);

    iInitialize(screenwidth, screenheight, "DX BALL");

    return 0;
}