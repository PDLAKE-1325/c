#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
int _getch()// #include <conio.h> [<termios.h>+<unistd.h>]
{
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
// ─────define
// color printf
#define Background "48;2"
#define Font "38;2"
#define cprint(style, r, g, b, str) printf("\033[" style ";"r";"g";"b"m" str "\033[0m");
// bool
#define true 1
#define false 0
//guitar
#define onUsingShop (!onPlanting&&!onDeleting&&!onUpgrading&&!onUtilities)
// ─────variable AJR
// input
char input;
int hanamja=false;
// player
int _playerPos[2] = {25,13};
int playerPos[2] = {25,13};
int stepped=0;
// status
int hp=5,wave=0;
double speed=0.5;
unsigned long long int coin=0,score=0,highscore=0;
int kill=0,enemy=0;
int isPlanting = false;
int isShopping = false;
int onWave = false;
int onPlanting = false;
int onUpgrading = false;
int onUtilities = false;
int onDeleting = false;
// map
int mapLoad[25][50]={ // [void 0] [wall 1] [player 2] [portal 3] [road 4] [bunker 5] [enemy 6] [tower 7]
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
int map[25][50]={
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,1},
    {1,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
int WJR[25][50]; // [type x] |  [enemy 1 ~ 4] [boss 5] 6~ tower ** tower-6 이 cbr_potap[i]의 i나타냄
// system pannel
int status[25][25]={
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,3,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,5,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,6,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,1},
    {1,0,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
int shop[25][25]={
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,3,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,5,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,13,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,12,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,1},
    {1,0,11,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
int selecting=0;
// intro,ending
char story[][100]={
    "[ 20xx. 3. 14. 대한민국 경기도 고양시 ]\n",
    "창틀에 누워 광합성을 하던 고양이 \033[38;2;100;200;50m샤미\033[0m는",
    "자신이 \033[38;2;250;50;50m좀비 바이러스\033[0m에 걸렸다는 것을 깨달았다.",
    "하지만 병원에 가기 싫었던 샤미는 그냥 잠을 잤고,","결국 세상에 좀비 바이러스가 퍼지게 된다.",
    "\n> press F to start"
};
char ending[][50]={
    "\033[38;2;200;100;100mGAME OVER...\033[0m\n",
    "[ Score ]"
};
// tower damage level
int damageLevel[49]={
    1,2,3,5,7,9,11,13,16,19,22,25,28,31,34,38,42,46,50,54,58,62,66,70,75,80,85,90,95,100,105,110,115,120,125,131,137,143,149,155,161,167,173,179,185,191,197,203,210}; //3 8 15 24 35 48 49
// ─────Structs
typedef struct e1
{
    int hp;
    int pos[2];
    int type;
    
}sEnemy;
typedef struct t1
{
    int planted;
    int pos[2];
    int level;
    int isTarget;
}sTower;
sTower cbr_potap[25];
sEnemy cbr_juk[100];
// ─────functions
// intro
void Story(){
    for (int s = 0; s < sizeof(story)/sizeof(story[0]); s++){
        for (int i = 0; i < 100; i++){
            system("clear");
            for (int j = 0; j < s; j++)
                printf("\033[0m%s\n\n",story[j]);
            if (story[s][i]=='\0'){
                printf("%s\n",story[s]);
                sleep(1);
                break;
            }for (int j = 0; j < i; j++)
                printf("%c",story[s][j]);
            printf("\n");
            usleep(9000);
        }
    }do{
        input=_getch();
    }while(input!='f');
}
void Home(){
    selecting=0;
    while (true){
        system("clear");
        printf("▏");
        cprint(Font,"255","50","50","                 AZTDG\n")
        printf("▏\n▏    Anyway, Zombie Tower Defense game\n▏       아무튼 좀비 타워디펜스 게임\n▏\n");
        if (selecting)
            printf("▏  ▷           게임 시작하기\n▏  ▶     하남자 모드");
        else
            printf("▏  ▶           게임 시작하기      [ ↩ ]\n▏  ▷     하남자 모드");
        if (hanamja){
            printf("[ ");
            cprint(Font,"200","150","50","켜짐")
            printf(" / 꺼짐]");
        }else{
            printf("[ 켜짐 / ");
            cprint(Font,"200","150","50","꺼짐")
            printf("]");
        }if (selecting)
            printf("[ ↩ ]");
        printf("\n▏\n▏");
        cprint(Font,"50","100","200"," 이동 [ 게임중: w,a,s,d | 현재화면: w,s ]\n")
        printf("▏");
        cprint(Font,"50","100","200"," 선택 [              Enter              ]\n")
        printf("▏\n▏High Score :");
        cprint(Font,"100","180","50"," ❦ ");
        unsigned long long int m=0;
        for (int n = 1;; n*=10){
            if (highscore/n==0)
                break;
            m++;
        }
        if (highscore<=0) 
            m=1;
        for (int n = m; n > 0; n--){
            printf("%llu",(highscore%(unsigned long long int)pow(10,n))/(unsigned long long int)pow(10,n-1));
            if (n%3==1&&n!=1)
                printf(",");
        }
        printf("\n\n");
        input=_getch();
        if (input=='w')
            selecting=0;
        else if (input=='s')
            selecting=1;
        else if (input==10){
            if (selecting){
                if (hanamja)
                    hanamja=false;
                else
                    hanamja=true;
            }else
                return;
        }
    }
}
// game phase
void GameStart(int cheatmode){
    for (int i = 0; i < 25; i++){
        cbr_potap[i].isTarget=-1;
        cbr_potap[i].level=0;
        cbr_potap[i].planted=false;
    }for (int i = 0; i < 25; i++)
        for (int j = 0; j < 50; j++){
            map[i][j]=mapLoad[i][j];
            WJR[i][j]=0;
        }
    playerPos[0]=25;
    playerPos[1]=13;
    _playerPos[0]=25;
    _playerPos[1]=13;
    isPlanting = false;
    onWave = false;
    stepped=0;
    wave=0;
    speed=0.5;
    score=0;
    kill=0;
    enemy=0;
    selecting=0;
    hp=5;
    if (cheatmode==true)
        coin=999999999;
    else
        coin=0;
}
void WaveEnemySystem(sEnemy juk[],int size){
    if(onWave&&kill==enemy||hp<=0){
        onWave=false;
        return;
    }for (int i = 0; i < size; i++){
        if((*(juk+i)).type==false) continue;
        if ((*(juk+i)).hp<=0){
            int rnd=rand()%5;
            coin+=pow((*(juk+i)).type,2)*(115+rnd)+pow(wave,2);
            score+=pow((*(juk+i)).type,2)*75*wave;
            (*(juk+i)).type=false;
            for (int l = 0; l < 25; l++)
                    if (cbr_potap[l].isTarget==i)
                            cbr_potap[l].isTarget=-1;
            map[(*(juk+i)).pos[1]][(*(juk+i)).pos[0]]=4;
            kill++;
            continue;
        }if(!((*(juk+i)).pos[0]==45&&(*(juk+i)).pos[1]==3))
            map[(*(juk+i)).pos[1]][(*(juk+i)).pos[0]]=4;
        if(((*(juk+i)).pos[1]-3)%8==0&&(*(juk+i)).pos[0]!=4){//3! 7 11! 15 19!
            if (map[(*(juk+i)).pos[1]][(*(juk+i)).pos[0]-1]!=6&&map[(*(juk+i)).pos[1]][(*(juk+i)).pos[0]-2]!=6)
                (*(juk+i)).pos[0]--;
        }else if(((*(juk+i)).pos[1]-7)%8==0&&(*(juk+i)).pos[0]!=45)
            (*(juk+i)).pos[0]++;
        else
            (*(juk+i)).pos[1]++; // - - - - - - - - 1c23dd
        for (int n = 0; n < 11; n++) //
            for (int m = 0; m < 7; m++)// +-4 +-2 / +-5 +-3 //14 23 23 21 14232
                if (WJR[(*(juk+i)).pos[1]-3+m][(*(juk+i)).pos[0]-5+n]>5){ //맵밖으로 나가는거 조심
                    if ((n>0&&n<10)&&(m>0&&m<6)){//123456789 12345 printf("2");
                        if (cbr_potap[WJR[(*(juk+i)).pos[1]-3+m][(*(juk+i)).pos[0]-5+n]-6].isTarget==-1)// printf("3");
                            cbr_potap[WJR[(*(juk+i)).pos[1]-3+m][(*(juk+i)).pos[0]-5+n]-6].isTarget=i;
                    }else{// printf("1");
                        if (cbr_potap[WJR[(*(juk+i)).pos[1]-3+m][(*(juk+i)).pos[0]-5+n]-6].isTarget==i)// printf("4");
                            cbr_potap[WJR[(*(juk+i)).pos[1]-3+m][(*(juk+i)).pos[0]-5+n]-6].isTarget=-1;
                    }
                }
        if(!((*(juk+i)).pos[0]==45&&(*(juk+i)).pos[1]==3)){
            if(!((*(juk+i)).pos[0]==4&&(*(juk+i)).pos[1]==21)){
                map[(*(juk+i)).pos[1]][(*(juk+i)).pos[0]]=6;
                WJR[(*(juk+i)).pos[1]][(*(juk+i)).pos[0]]=(*(juk+i)).type;
            }else{
                hp--;
                (*(juk+i)).type=false;
                for (int l = 0; l < 25; l++)
                    if (cbr_potap[l].isTarget==i)
                            cbr_potap[l].isTarget=-1;
                int rnd=rand()%5;
                coin+=pow((*(juk+i)).type,2)*(115+rnd)+pow(wave,2);
                kill++;
            }
        }
    }
}
void PotapSystem(){
    if(hp<=0)
        return;
    for (int i = 0; i < 25; i++)
        if (cbr_potap[i].planted==true)
            if (cbr_potap[i].isTarget!=-1) //printf("Damaged");
                cbr_juk[cbr_potap[i].isTarget].hp-=damageLevel[cbr_potap[i].level-1];
}
void PointerMove(int x, int y, int changeAxis,int k){
    if (isShopping)
        return;
    if (map[_playerPos[1]+x][_playerPos[0]+y]!=1){
        playerPos[changeAxis]+=k;
        while (true){
            if (!(map[playerPos[1]][playerPos[0]]==0||map[playerPos[1]][playerPos[0]]==7)){
                playerPos[changeAxis]+=k;
                continue;
            }
            break;
        }
    }
    map[_playerPos[1]][_playerPos[0]]=stepped;
    stepped=map[playerPos[1]][playerPos[0]];
    _playerPos[0] = playerPos[0];
    _playerPos[1] = playerPos[1];
    map[playerPos[1]][playerPos[0]]=2;
}
void Control(){
    int input=_getch();
    switch (input){
        case 'w':
            if (isShopping&&selecting!=0&&!onPlanting&&!onDeleting&&!onUpgrading)
                selecting--;
            else
                PointerMove(-1,0,1,-1);
            break;
        case 'a':
            if (onUtilities&&selecting==2){
                if (speed>0.025){
                    speed-=0.025;
                    if (speed<0.025)
                        speed=0.025;
                }
            }else
                PointerMove(0,-1,0,-1);
            break;
        case 's':
            if (isShopping&&selecting!=3&&!onPlanting&&!onDeleting&&!onUpgrading)
                if (onUtilities){
                    if(selecting!=2)
                        selecting++;
                }else
                    selecting++;
            else
                PointerMove(1,0,1,1);
            break;
        case 'd':
            if (onUtilities&&selecting==2){
                if (speed<1.25){
                    speed+=0.025;
                    if (speed>1.25)
                        speed=1.25;
                }
            }else
                PointerMove(0,1,0,1);
            break;
        case 'f':
            if(!isShopping){
                isShopping=true;
                if (WJR[_playerPos[1]][_playerPos[0]]!=0)
                    isPlanting=true;
            }
            break;
        case 'q':
            if (isShopping){
                if (onPlanting){
                    isPlanting=false;
                    onPlanting=false;
                }else if(onUpgrading||onDeleting){
                    onUpgrading=false;
                    onDeleting=false;
                }else if(onUtilities){
                    selecting=0;
                    onUtilities=false;
                    if (WJR[_playerPos[1]][_playerPos[0]]!=0)
                        isPlanting=true;
                }else{
                    isShopping = false;
                    isPlanting = false;
                }
            }
            break;
        case 10:
            if(!onWave&&!isShopping){
                onWave=2; 
                wave++; 
                kill=0;;
            }else if(!onWave&&isShopping){
                if (onPlanting){
                    int ptn=0;
                    for (int k = 0; k < 25; k++)
                        if (cbr_potap[k].planted==true)
                            ptn+=1000;
                    if (coin>=ptn)
                        for (int x = 0; x < 25; x++)
                            if (cbr_potap[x].planted==false){
                                coin-=ptn;
                                cbr_potap[x].planted=true;
                                cbr_potap[x].isTarget=-1;
                                cbr_potap[x].level=1;
                                stepped=7;
                                WJR[_playerPos[1]][_playerPos[0]]=6+x;
                                isShopping=false;
                                isPlanting=false;
                                onPlanting=false;
                                break;
                            }
                }else if(onUpgrading&&cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level!=49){
                    if (coin>=cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level*750){
                        coin-=cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level*750;
                        cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level++;
                    }
                }else if (onUtilities){
                    if (selecting==0){
                        if (hp!=5&&coin>=1500){
                            hp++;
                            coin-=1500;
                        }
                    }else if (selecting==1){
                        if (hp!=1){
                            hp--;
                            coin+=1000;
                        }
                    } 
                }else if(onDeleting){
                    coin+=500;
                    cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].planted=false;
                    cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].isTarget=-1;
                    cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level=0;
                    stepped=0;
                    WJR[_playerPos[1]][_playerPos[0]]=0;
                    selecting=0;
                    isShopping=false;
                    isPlanting=false;
                    onDeleting=false;
                }else{
                    if(selecting==0){
                        if (!isPlanting){
                            onPlanting=true;
                            isPlanting=true;
                        }
                    }else if(selecting==1){
                        if (isPlanting)
                            onUpgrading=true;  
                    }else if(selecting==2){
                        onUtilities=true;
                        isPlanting=false;
                    }else
                        if (isPlanting)
                            onDeleting=true;
                    selecting=0;
                }
            }
        default:
             break;
    }
}
void Load(){
    system("clear");
    for (int i = 0; i < 25; i++){
        for (int j = 0; j < 50; j++){
            if((playerPos[0]+5)>j&&(playerPos[0]-5)<j&&(playerPos[1]+3)>i&&(playerPos[1]-3)<i&&isPlanting){
                if(map[i][j]==1){
                    cprint(Background,"155","0","0","▓")
                }else if(map[i][j]==2){                    
                    cprint(Background,"155","0","0","⤬")
                }else if(map[i][j]==3){
                    cprint(Background,"155","0","0","∑")
                }else if(map[i][j]==4){
                    cprint(Background,"155","0","0","░")
                }else if(map[i][j]==5){
                    cprint(Background,"155","0","0","⏏")
                }else if(map[i][j]==7){
                    cprint(Background,"155","0","0","⌖")
                }else{
                    cprint(Background,"155","0","0"," ")
                }
            }else{
                if(map[i][j]==1){
                    printf("▓");
                }else if(map[i][j]==2){
                    if (isShopping){
                        if (onUtilities)
                            cprint(Font,"255","255","0","¤")
                        else{
                            if (WJR[i][j])
                                cprint(Font,"200","150","50","✚")
                            else
                                printf("✚");
                        }
                    }else if(onWave)
                        printf("⚑");
                    else if (WJR[i][j])
                        cprint(Font,"200","150","50","○")
                    else 
                        printf("○");
                }else if(map[i][j]==3){
                    cprint(Background,"100","000","255","<")
                }else if(map[i][j]==4){
                    cprint(Background,"100","100","0","░")
                }else if(map[i][j]==5){
                    cprint(Font,"0","0","200","⏏")
                }else if(map[i][j]==6){
                    if (WJR[i][j]==1)
                        cprint(Background,"120","120","0","☠")
                    else if (WJR[i][j]==2)
                        cprint(Background,"120","120","0","☼")
                    else if (WJR[i][j]==3)
                        cprint(Background,"120","120","0","☫")
                    else if (WJR[i][j]==4)
                        cprint(Background,"120","120","0","࿗")
                    else if (WJR[i][j]==5)
                        cprint(Background,"120","120","0","֎")
                    else
                        cprint(Background,"100","100","0","░")
                }else if(map[i][j]==7){
                    if(cbr_potap[WJR[i][j]-6].level==49)
                        printf("⌖");
                    else if(cbr_potap[WJR[i][j]-6].level>=35)
                        cprint(Font,"100","50","255","⌖")                 
                    else if(cbr_potap[WJR[i][j]-6].level>=24)
                        cprint(Font,"0","100","255","⌖")
                    else if(cbr_potap[WJR[i][j]-6].level>=15)
                        cprint(Font,"100","255","0","⌖")
                    else if(cbr_potap[WJR[i][j]-6].level>=8)
                        cprint(Font,"255","255","0","⌖")
                    else if(cbr_potap[WJR[i][j]-6].level>=3)
                        cprint(Font,"255","100","0","⌖")
                    else if (cbr_potap[WJR[i][j]-6].level>=0)
                        cprint(Font,"255","0","0","⌖")
                }else{
                    printf(" ");
                }
            }   
        }
        printf(" ");//tag
        if (!isShopping){
            for (int j = 0; j < 25; j++){
                if(status[i][j]==1){
                    printf("▓");
                }else if(status[i][j]==2){
                    printf("WAVE  %-11d",wave);
                }else if(status[i][j]==3){
                    printf("[ %5d / %-5d ]",kill,enemy);
                }else if(status[i][j]==4){
                    cprint(Font,"200","50","0","HP  ")
                    for (int n = 1; n <= 5; n++)
                       printf("\033[38;2;255;0;0m%s \033[0m",((hp>=n)?("♥"):("♡"))); 
                }else if(status[i][j]==5){
                    cprint(Font,"200","200","0","¤ ")
                    unsigned long long int m=0,o=0;
                    for (int n = 1;; n*=10){
                        if (coin/n==0)
                            break;
                        m++;
                    }
                    if (coin<=0)
                        m=1;
                    for (int n = m; n > 0; n--){
                        printf("%llu",(coin%(unsigned long long int)pow(10,n))/(unsigned long long int)pow(10,n-1));
                        if (n%3==1&&n!=1){
                            printf(",");
                            o++;
                        }
                    }for (int n = 0; n < 15-m-o; n++)
                        printf(" ");
                }else if(status[i][j]==6){
                    printf("< Score >");
                }else if(status[i][j]==7){
                    cprint(Font,"100","180","50","❦ ");
                    unsigned long long int m=0,o=0;
                    for (int n = 1;; n*=10){
                        if (score/n==0) break;
                        m++;
                    }
                    if (score<=0)
                        m=1;
                    for (int n = m; n > 0; n--){
                        printf("%llu",(score%(unsigned long long int)pow(10,n))/(unsigned long long int)pow(10,n-1));
                        if (n%3==1&&n!=1){
                            printf(",");
                            o++;
                        }
                    }for (int n = 0; n < 15-m-o; n++)
                        printf(" ");
                }else if(status[i][j]==8){
                    printf("─");
                }else if(status[i][j]==9){
                    printf("");
                }else if(status[i][j]==10){
                    if(!onWave){
                        cprint(Font,"200","150","50","( F )")
                        printf(" 상점 열기      ");
                    }else
                        cprint(Font,"200","150","50","[!] 웨이브 진행중    ")
                }else if(status[i][j]==11){
                    if (!onWave){
                        cprint(Font,"200","150","50","( ↩ )")
                        printf(" 웨이브 시작    ");
                    }else
                        cprint(Font,"200","150","50","[!] 상점 사용 불가   ")
                }else{
                    printf(" ");
                }
            }
        }else{
            for (int j = 0; j < 25; j++){
                if(shop[i][j]==1){
                    printf("▓");
                }else if(shop[i][j]==2){
                    cprint(Font,"200","200","0","¤ ")
                    unsigned long long int m=0,o=0;
                    for (int n = 1;; n*=10){
                        if (coin/n==0) break;
                        m++;
                    }
                    if (coin<=0)
                        m=1;
                    for (int n = m; n > 0; n--){
                        printf("%llu",(coin%(unsigned long long int)pow(10,n))/(unsigned long long int)pow(10,n-1));
                        if (n%3==1&&n!=1){
                            printf(",");
                            o++;
                        }
                    }for (int n = 0; n < 15-m-o; n++)
                        printf(" ");
                }else if(shop[i][j]==3){
                    if (onPlanting){
                        cprint(Font,"200","0","0","⌖ Lv.1           ")
                    }else if(onUpgrading){
                        if (cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level==49)
                            printf("\033[38;2;200;150;50m⌖ Lv.max         \033[0m");
                        else
                            printf("\033[38;2;200;150;50m⌖ Lv.%-2d >  Lv.%-2d \033[0m",cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level,cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level+1);
                    }else if(onUtilities){
                        printf("  ◈ 기능 목록 ◈  ");
                    }else if(onDeleting){
                        if (cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level==49)
                            printf("\033[38;2;200;150;50m⌖ Lv.max         \033[0m");
                        else
                            printf("\033[38;2;200;150;50m⌖ Lv.%-2d          \033[0m",cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level);
                    }else{
                        if (!isPlanting){
                            if(selecting==0)
                                cprint(Background,"155","0","0"," ⌘ Tower         ")
                            else
                                cprint(Font,"200","0","0"," ⌘ Tower         ")
                        }else{
                            if(selecting==0)
                                cprint(Background,"50","50","50"," ⌘ Tower         ")
                            else
                                cprint(Font,"20","20","20"," ⌘ Tower         ")
                        }
                    }
                }else if(shop[i][j]==4){
                    if (onPlanting){
                        printf("  ◈ 포탑 구입 ◈  ");
                    }else if (onUpgrading){
                        printf("  ◈ 포탑 강화 ◈  ");
                    }else if (onUtilities){
                        if (selecting==0)
                            printf("▶ ");
                        else
                            printf("▷ ");
                        cprint(Font,"255","0","0","♥")
                        printf(" +1 |");
                        cprint(Font,"200","200","0"," ¤ ");
                        printf("-1500 ");
                    }else if (onDeleting)
                        printf("  ◈ 포탑 철거 ◈  ");
                    else
                        if (isPlanting){
                            if(selecting==1)
                                cprint(Background,"200","155","0"," ⎇ Upgrade       ")
                            else
                                cprint(Font,"200","155","0"," ⎇ Upgrade       ")
                        }else{
                            if(selecting==1)
                                cprint(Background,"50","50","50"," ⎇ Upgrade       ")
                            else
                                cprint(Font,"20","20","20"," ⎇ Upgrade       ")
                        }
                }else if(shop[i][j]==5){
                    if (onPlanting)
                        printf(" 현재 위치에 설치");
                    else if(onUpgrading)
                        printf(" 선택된 포탑 강화"); // 데미지 +1;
                    else if(onUtilities){
                        if (selecting==1)
                            printf("▶ ");
                        else
                            printf("▷ ");
                        cprint(Font,"255","0","0","♥")
                        printf(" -1 |");
                        cprint(Font,"200","200","0"," ¤ ");
                        printf("+1000 ");
                    }else if(onDeleting)
                        printf(" 선택된 포탑 철거");
                    else{
                        if(selecting==2)
                            cprint(Background,"0","155","0"," ⛨ Utility       ")
                        else
                            cprint(Font,"0","155","0"," ⛨ Utility       ")
                    }                  
                }else if(shop[i][j]==6){
                    if (selecting==0&&onUsingShop)
                        printf("▼ (S) ");
                    else if (selecting==1&&onUsingShop)
                        printf("▲ (W) ");
                    else
                        printf("      ");
                }else if(shop[i][j]==7){
                    if (selecting==1&&onUsingShop)
                        printf("▼ (S) ");
                    else if (selecting==2&&onUsingShop)
                        printf("▲ (W) ");
                    else
                        printf("      ");
                }else if(shop[i][j]==14){
                    if (selecting==2&&onUsingShop)
                        printf("▼ (S) ");
                    else if (selecting==3&&onUsingShop)
                        printf("▲ (W) ");
                    else
                        printf("      ");
                }else if(shop[i][j]==8){
                    printf("─");
                }else if(shop[i][j]==9){
                    printf("");
                }else if(shop[i][j]==10){
                    cprint(Font,"200","150","50","( Q )")
                    if (onUsingShop)
                        printf(" 상점 닫기      ");
                    else
                        printf(" 뒤로가기       ");                    
                }else if(shop[i][j]==11){
                    cprint(Font,"200","150","50","( ↩ )")
                    if (onPlanting)
                        printf(" 포탑 설치      ");
                    else if (onUpgrading)
                        printf(" 포탑 강화      ");
                    else if (onUtilities)
                        printf(" 항목 구입      ");
                    else if (onDeleting)
                        printf(" 포탑 철거      ");
                    else
                        printf(" 항목 선택      ");
                }else if(shop[i][j]==12){ 
                    if (onPlanting){
                        int ptn=0;
                        for (int k = 0; k < 25; k++)
                            if (cbr_potap[k].planted==true)
                                ptn+=1000;
                        cprint(Font,"200","200","0"," ¤ ")
                        printf("-");
                        unsigned long long int m=0,o=0;
                        for (int n = 1;; n*=10){
                            if (ptn/n==0) break;
                            m++;
                        }
                        if (ptn<=0)
                            m=1;
                        for (int n = m; n > 0; n--){
                            printf("%llu",(ptn%(unsigned long long int)pow(10,n))/(unsigned long long int)pow(10,n-1));
                            if (n%3==1&&n!=1){
                                printf(",");
                                o++;
                            }
                        }
                        for (int n = 0; n < 15-m-o; n++)
                            printf(" ");
                    }else if(onUpgrading){
                        cprint(Font,"200","200","0"," ¤ ")
                        if (cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level==49)
                            printf("강화불가        ");
                        else
                            printf("-%-12d   ",cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level*750);
                    }else if(onUtilities){
                        printf("\033[38;2;0;100;255m 현재 배속 x%-7.3lf\033[0m",speed);
                    }else if(onDeleting){
                        cprint(Font,"200","200","0","      ¤ ")
                        printf("+500       ");
                    }else{
                        if (WJR[_playerPos[1]][_playerPos[0]]){
                            printf("\033[38;2;200;150;50m◈ 포탑 Lv.%-2d 선택됨\033[0m",cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level);
                        }else
                            cprint(Font,"100","150","250"," ◈ 선택된 포탑 없음")
                        }
                }else if(shop[i][j]==13){
                    if (onPlanting){
                        int ptn=0;
                        for (int k = 0; k < 25; k++)
                            if (cbr_potap[k].planted==true)
                                ptn++;
                        printf("   [ %2d / 25 ]   ",ptn);
                    }else if(onUpgrading){
                        if (cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level==49)
                            printf("    [ ⚔ 210 ]    ");
                        else
                            printf(" [ ⚔%3d > ⚔%3d ] ",damageLevel[cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level-1],damageLevel[cbr_potap[WJR[_playerPos[1]][_playerPos[0]]-6].level]);
                    }else if (onUtilities){
                        if (selecting==2)
                            printf("▶ ");
                        else
                            printf("▷ ");
                        cprint(Font,"0","100","225","⏭")
                        printf(" | ");
                        printf("+ / - [a/d]");
                    }else if(onDeleting){
                        int ptn=0;
                        for (int k = 0; k < 25; k++)
                            if (cbr_potap[k].planted==true)
                                ptn++;
                        printf("   [ %2d / 25 ]   ",ptn);
                    }else{
                        if (isPlanting){
                            if(selecting==3)
                                cprint(Background,"200","100","200"," ⛒ Delete        ")
                            else
                                cprint(Font,"200","100","200"," ⛒ Delete        ")
                        }else{
                            if(selecting==3)
                                cprint(Background,"50","50","50"," ⛒ Delete        ")
                            else
                                cprint(Font,"20","20","20"," ⛒ Delete        ")
                        }
                    }
                }else{
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("\n\n\n\n\n\n\n\n");
}
void WaveSystem(){
    int i,j=0, waveDesign[5]={};
    waveDesign[0]+=(1+(wave>20)+(wave>40))*5*(wave%5);
    if (wave%5==0){//1 1 1 2
        waveDesign[1]+=1*(wave%10!=0);//123456789...
        waveDesign[2]+=(wave>=10)*(wave%20!=0);//2468...
        waveDesign[3]+=(wave>=20)*(wave%30!=0);//48..
        waveDesign[4]+=(wave>=30);//1020..
    }for (i = 0; i < waveDesign[0]; i++){
        cbr_juk[i].hp=(int)(2*pow(1.15,wave));
        cbr_juk[i].pos[0]=45;
        cbr_juk[i].pos[1]=3;
        cbr_juk[i].type=1;
    }
    j=i;
    for (i = i; i < waveDesign[1]+j; i++){
        cbr_juk[i].hp=(int)(5*pow(1.2,wave));
        cbr_juk[i].pos[0]=45;
        cbr_juk[i].pos[1]=3;
        cbr_juk[i].type=2;
    }
    j=i;
    for (i = i; i < waveDesign[2]+j; i++){
        cbr_juk[i].hp=(int)(7*pow(1.2,wave));
        cbr_juk[i].pos[0]=45;
        cbr_juk[i].pos[1]=3;
        cbr_juk[i].type=3;
    }
    j=i;
    for (i = i; i < waveDesign[3]+j; i++){
        cbr_juk[i].hp=(int)(9*pow(1.2,wave));
        cbr_juk[i].pos[0]=45;
        cbr_juk[i].pos[1]=3;
        cbr_juk[i].type=4;
    }
    j=i;
    for (i = i; i < waveDesign[4]+j; i++){
        cbr_juk[i].hp=(int)(15*pow(1.2,wave));
        cbr_juk[i].pos[0]=45;
        cbr_juk[i].pos[1]=3;
        cbr_juk[i].type=5;
    }
    enemy=i;
    onWave=true;
}
void PlayPhase(){
    while (true){
        Load();
        if(hp<=0){
            sleep(2);
            system("clear");
            return;
        }if(!onWave){
            Control();
        }else{
            if(onWave==true){
                WaveEnemySystem(cbr_juk,enemy);
                PotapSystem();
            }else
                WaveSystem();
            usleep(500000*speed);
        }
    }
}
// end phase
void End(){
    for (int s = 0; s < 2; s++){
        for (int i = 0; i < 100; i++){
            system("clear");
            for (int j = 0; j < s; j++)
                printf("\033[0m%s\n\n",ending[j]);
            if (ending[s][i]=='\0'){
                printf("%s\n",ending[s]);
                sleep(1);
                break;
            }for (int j = 0; j < i; j++)
                printf("%c",ending[s][j]);
            printf("\n");
            usleep(12000);
        }
    }
    cprint(Font,"100","180","50","\n ❦ ");
    unsigned long long int m=0;
    for (int n = 1;; n*=10){
        if (score/n==0) break;
        m++;
    }if (score<=0) 
        m=1;
    for (int n = m; n > 0; n--){
        printf("%llu",(score%(unsigned long long int)pow(10,n))/(unsigned long long int)pow(10,n-1));
        if (n%3==1&&n!=1)
            printf(",");
    }if (score>highscore){
        cprint(Font,"200","150","50"," [ New Record ]")
        highscore=score;
    }
    printf("\n\n\n");
    sleep(1);
    printf("> press F to start\n\n");
    do{
        input=_getch();
    } while (input!='f');
}
// ──────────────────────────────────── main
int main(){
    Story();
    while (true){
        Home();
        srand(time(NULL));
        GameStart(hanamja);
        PlayPhase();
        End();
    }
}
// 끝