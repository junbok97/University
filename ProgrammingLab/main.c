//1692136 이준복
//2인용 snake 게임

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2		'w'//player2 는 AWSD 로 방향키 대신
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

#define WIDTH 120
#define HEIGHT 35
#define SNAKE_LENGTH 6
#define SNAKE_LENGTH_MAX 100
#define SIZE 10

int Delay = 100;
int keep_moving = 1; // 1:계속이동, 0:한칸씩이동.
int time_out = 60; // 제한시간

int items[WIDTH][HEIGHT] = { 0, };
int item_count = 0;
int iteminterval = 5;
int less[WIDTH][HEIGHT] = { 0, };
int less_count = 0;
int lessinterval = 10;
int fast[WIDTH][HEIGHT] = { 0, };
int fast_count = 0;
int fastinterval = 7;
int slow[WIDTH][HEIGHT] = { 0, };
int slow_count = 0;
int slowinterval = 10;

int potalinterval = 5;

int called[2];
int winner = 0;

int frame_count = 0; // game 진행 frame count 로 속도 조절용으로 사용된다.
int p1_frame_sync = 10;
int p2_frame_sync = 10;
int item_frame_sync = 50;
int less_frame_sync = 50;
int fast_frame_sync = 50;
int slow_frame_sync = 50;

int map[35][120] =
{

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

};
void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}

void gotoxy2(COORD pos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

typedef struct {
	COORD snake[SNAKE_LENGTH_MAX];
	int length;
	int dir;
	int player;
	int life;
	int score;
}SNAKE;

typedef struct {
	int x[SIZE];
	int y[SIZE];
	int dir[SIZE];
	int front;
	int rear;

}TAIL;

// player 2
SNAKE snake_1;
TAIL tail_1;

//player 2
SNAKE snake_2;
TAIL tail_2;

void Set_snake(SNAKE* s, int player, int dir) {
	s->length = SNAKE_LENGTH;
	s->player = player;
	s->dir = dir;
	s->life = 3;
	s->score = 0;
	for (int i = 0; i < SNAKE_LENGTH_MAX; i++) {
		s->snake[i].X = 0;
		s->snake[i].Y = 0;
	}

}

void Set_tail(TAIL* t) {
	t->front = 0;
	t->rear = 0;
	for (int i = 0; i < SIZE; i++) {
		t->x[i] = t->y[i] = t->dir[i] = 0;
	}
}

void show_tail(SNAKE* s, TAIL* t, int i) {

	if (t->x[t->front] == s->snake[i].X && t->y[t->front] == s->snake[i].Y) {
		s->dir = t->dir[t->front];
		t->front++;
	}

	gotoxy2(s->snake[i]);
	switch (s->dir) {
	case 0:
		printf("▼");
		break;
	case 1:
		printf("▲");
		break;
	case 2:
		printf("▶");
		break;
	case 3:
		printf("◀");
		break;
	}

	if (t->front == 10)
		t->front = 0;
}

void show_snake(SNAKE* s, TAIL* t) {

	int i;

	switch (s->player) {
	case 1:
		textcolor(BLUE2, BLACK);
		gotoxy2(s->snake[0]); printf("①");
		break;
	case 2:
		textcolor(RED2, BLACK);
		gotoxy2(s->snake[0]); printf("②");
		break;
	}

	
	// 몸통
	for (i = 1; i < s->length - 1; i++) {
		gotoxy2(s->snake[i]);
		printf("■"); 
	}
	show_tail(s, t, i);
	textcolor(WHITE, BLACK);
}

int move_snake(int x, int y, SNAKE* s, TAIL* t, SNAKE* p)
{
	int i;

	// 부딪혔는지
	for (i = 0; i < s->length; i++) {
		if (x == p->snake[i].X && y == p->snake[i].Y)
			return 0;
	}

	//꼬리
	gotoxy2(s->snake[s->length - 1]);
	printf("  ");

	//몸통
	for (i = s->length - 1; 1 <= i; i--) {
		s->snake[i] = s->snake[i - 1];
	}
	s->snake[0].X = x; s->snake[0].Y = y;

	show_snake(s, t);
	return 1;
}

#define ITEM	"★"
void show_item()
{
	int x, y;

	while (1) {
		x = rand() % (WIDTH - 20) + 10;
		if (x % 2 == 1)
			x++;
		y = rand() % (HEIGHT - 10) + 5;
		if (items[x][y] == 0)
		{
			items[x][y] = 1;
			break;
		}
	}
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(ITEM);
	item_count++;
	textcolor(WHITE, BLACK);
}
void move_item() {
	int x, y, dx, dy, newx, newy;
	int newitems[WIDTH][HEIGHT] = { 0 };



	if (item_count == 0)
		return;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (items[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = rand() % 3 - 1; // -1 0 1
				newx = x + dx;
				if (newx % 2 == 1)
					newx++;
				newy = y + dy;
				if (newx > WIDTH - 4) newx = WIDTH - 5;
				if (newx < 10) newx = 20;
				if (newy > HEIGHT - 4) newy = HEIGHT - 5;
				if (newy < 4) newy = 4;
				gotoxy(x, y);
				textcolor(BLACK, BLACK);
				printf("  "); // erase gold
				textcolor(YELLOW2, BLACK);
				gotoxy(newx, newy);
				printf(ITEM);
				newitems[newx][newy] = 1; // 이동된 golds의 좌표
				textcolor(WHITE, BLACK);
			}
		}
	}
	memcpy(items, newitems, sizeof(newitems)); // 한번에 gold 위치를 조정한다.
}

#define LESS "※"
void show_less() {

	int x, y;

	while (1) {
		x = rand() % (WIDTH - 20) + 10;
		if (x % 2 == 1)
			x++;
		y = rand() % (HEIGHT - 10) + 5;
		if (less[x][y] == 0)
		{
			less[x][y] = 1;
			break;
		}
	}
	textcolor(RED2, BLACK);
	gotoxy(x, y);
	printf(LESS);
	less_count++;
	textcolor(WHITE, BLACK);
}
void move_less() {
	int x, y, dx, dy, newx, newy;
	int newless[WIDTH][HEIGHT] = { 0 };


	if (less_count == 0)
		return;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (less[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = rand() % 3 - 1; // -1 0 1
				newx = x + dx;
				if (newx % 2 == 1)
					newx++;
				newy = y + dy;
				if (newx > WIDTH - 4) newx = WIDTH - 5;
				if (newx < 10) newx = 20;
				if (newy > HEIGHT - 4) newy = HEIGHT - 5;
				if (newy < 4) newy = 4;
				gotoxy(x, y);
				textcolor(BLACK, BLACK);
				printf("  "); // erase gold
				textcolor(RED2, BLACK);
				gotoxy(newx, newy);
				printf(LESS);
				newless[newx][newy] = 1; // 이동된 golds의 좌표
				textcolor(WHITE, BLACK);
			}
		}
	}
	memcpy(less, newless, sizeof(newless)); // 한번에 gold 위치를 조정한다.
}

#define FAST "≫"
void show_fast()
{
	int x, y;

	while (1) {
		x = rand() % (WIDTH - 20) + 10;
		if (x % 2 == 1)
			x++;
		y = rand() % (HEIGHT - 10) + 5;
		if (fast[x][y] == 0)
		{
			fast[x][y] = 1;
			break;
		}
	}

	textcolor(GREEN2, BLACK);
	gotoxy(x, y);
	printf(FAST);
	fast_count++;
	textcolor(WHITE, BLACK);
}
void move_fast() {
	int x, y, dx, dy, newx, newy;
	int newfast[WIDTH][HEIGHT] = { 0 };


	if (fast_count == 0)
		return;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (fast[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = rand() % 3 - 1; // -1 0 1
				newx = x + dx;
				if (newx % 2 == 1)
					newx++;
				newy = y + dy;
				if (newx > WIDTH - 4) newx = WIDTH - 5;
				if (newx < 10) newx = 20;
				if (newy > HEIGHT - 4) newy = HEIGHT - 5;
				if (newy < 4) newy = 4;
				gotoxy(x, y);
				textcolor(BLACK, BLACK);
				printf("  "); // erase gold
				textcolor(GREEN2, BLACK);
				gotoxy(newx, newy);
				printf(FAST);
				newfast[newx][newy] = 1; // 이동된 golds의 좌표
				textcolor(WHITE, BLACK);
			}
		}
	}
	memcpy(fast, newfast, sizeof(newfast)); // 한번에 gold 위치를 조정한다.
}

#define SLOW "≪"
void show_slow()
{
	int x, y;

	while (1) {
		x = rand() % (WIDTH - 20) + 10;
		if (x % 2 == 1)
			x++;
		y = rand() % (HEIGHT - 10) + 5;
		if (slow[x][y] == 0)
		{
			slow[x][y] = 1;
			break;
		}
	}

	textcolor(MAGENTA2, BLACK);
	gotoxy(x, y);
	printf(SLOW);
	slow_count++;
	textcolor(WHITE, BLACK);
}
void move_slow() {
	int x, y, dx, dy, newx, newy;
	int newslow[WIDTH][HEIGHT] = { 0 };


	if (slow_count == 0)
		return;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (slow[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = rand() % 3 - 1; // -1 0 1
				newx = x + dx;
				if (newx % 2 == 1)
					newx++;
				newy = y + dy;
				if (newx > WIDTH - 4) newx = WIDTH - 5;
				if (newx < 10) newx = 20;
				if (newy > HEIGHT - 4) newy = HEIGHT - 5;
				if (newy < 4) newy = 4;
				gotoxy(x, y);
				textcolor(BLACK, BLACK);
				printf("  "); // erase gold
				textcolor(MAGENTA2, BLACK);
				gotoxy(newx, newy);
				printf(SLOW);
				newslow[newx][newy] = 1; // 이동된 golds의 좌표
				textcolor(WHITE, BLACK);
			}
		}
	}
	memcpy(slow, newslow, sizeof(newslow)); // 한번에 gold 위치를 조정한다.
}

#define LIFE "♥"
#define LIFE2 "♡"
void show_life(SNAKE* s) {

	textcolor(RED2, BLACK);
	int i;
	switch (s->player) {

	case 1:

		for (i = 1; i < s->life + 1; i++) {
			gotoxy(80 + (i * 2), 1);
			printf(LIFE);
		}

		for (; i < 4; i++) {
			gotoxy(80 + (i * 2), 1);
			printf(LIFE2);
		}
		break;

	case 2:

		for (i = 1; i < s->life + 1; i++) {
			gotoxy(20 + (i * 2), 1);
			printf(LIFE);
		}

		for (; i < 4; i++) {
			gotoxy(20 + (i * 2), 1);
			printf(LIFE2);
		}

	}
	textcolor(WHITE, BLACK);
}
void show_score(SNAKE* s) {

	textcolor(WHITE, BLACK);
	switch (s->player) {
	case 1:
		gotoxy(80, 0);
		printf("Player 1 : %d", s->score);
		break;
	case 2:
		gotoxy(20, 0);
		printf("Player 2 : %d", s->score);
		break;
	}

}


typedef struct {
	COORD in[3];
	COORD out[3];
	int way;
}POTAL;

POTAL potal;

void make_potal() {

	int inx, iny, outx, outy;

	while (1) {
		inx = rand() % 50 + 5;
		if (inx % 2 == 1)
			inx++;
		iny = rand() % 10 + 8;
		outx = rand() % 50 + 40;
		if (outx % 2 == 1)
			outx++;
		outy = rand() % 10 + 16;

		if (inx != outx && iny != outy)
			break;
	}

	int i;

	for (i = 0; i < 3; i++) {
		textcolor(BLACK, BLACK);
		gotoxy2(potal.in[i]);
		printf("  ");
		gotoxy2(potal.out[i]);
		printf("  ");
	}

	switch (rand() % 2) {
	case 0: // 세로 세로
		for (i = 0; i < 3; i++) {
			potal.in[i].X = inx;
			potal.in[i].Y = iny - 1 + i;
			potal.out[i].X = outx;
			potal.out[i].Y = outy - 1 + i;
		}
		potal.way = 0;
		break;

	case 1: // 가로 가로
		for (i = 0; i < 3; i++) {
			potal.in[i].X = inx - 2 + (i * 2);
			potal.in[i].Y = iny;
			potal.out[i].X = outx - 2 + (i * 2);
			potal.out[i].Y = outy;
		}
		potal.way = 1;
		break;
	}

	textcolor(CYAN2, BLACK);
	gotoxy2(potal.in[i]);
	printf("▒");
	gotoxy2(potal.out[i]);
	printf("▒");

}
void show_potal() {

	if (potal.in->X == 0)
		return;

	for (int i = 0; i < 3; i++) {
		textcolor(CYAN2, BLACK);
		gotoxy2(potal.in[i]);
		printf("▒");
		gotoxy2(potal.out[i]);
		printf("▒");
	}
}
void move_potal(int* x, int* y, int dir, int inout) {
	//1 은 in으로 들어온거 2는 out으로 들어온거
	switch (inout) {
	case 1:
		switch (potal.way) {
		case 0:
			switch (dir) {
			case 2:
				*x = potal.out[1].X - 2;
				*y = potal.out[1].Y;
				break;
			case 3:
				*x = potal.out[1].X + 2;
				*y = potal.out[1].Y;
				break;
			}
			break;

		case 1:
			switch (dir) {
			case 0:
				*x = potal.out[1].X;
				*y = potal.out[1].Y - 1;
				break;
			case 1:
				*x = potal.out[1].X;
				*y = potal.out[1].Y + 1;
				break;
			}
			break;
		}
		break;

	case 2:
		switch (potal.way) {
		case 0:
			switch (dir) {
			case 2:
				*x = potal.in[1].X - 2;
				*y = potal.in[1].Y;
				break;
			case 3:
				*x = potal.in[1].X + 2;
				*y = potal.in[1].Y;
				break;
			}
			break;

		case 1:
			switch (dir) {
			case 0:
				*x = potal.in[1].X;
				*y = potal.in[1].Y - 1;
				break;
			case 1:
				*x = potal.in[1].X;
				*y = potal.in[1].Y + 1;
				break;
			}
			break;
		}
		break;
	}
}

void draw_box(int x1, int y1, int x2, int y2, char ch)
{
	int x, y;
	for (x = x1; x <= x2; x += 1) {
		gotoxy(x, y1);
		printf("%c", ch);
		gotoxy(x, y2);
		printf("%c", ch);
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%c", ch);
		gotoxy(x2, y);
		printf("%c", ch);
	}
}
void draw_box2(int x1, int y1, int x2, int y2, char* ch)
{
	int x, y;
	int len = strlen(ch);
	for (x = x1; x <= x2; x += len) {
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
}
void draw_hline(int y, int x1, int x2, char ch)
{
	gotoxy(x1, y);
	for (; x1 <= x2; x1++)
		putchar(ch);
}

void init_game()
{
	int x, y;
	char cmd[100];

	srand(time(NULL));
	winner = 0;
	snake_1.length = snake_2.length = SNAKE_LENGTH;
	called[0] = called[1] = 0;
	for (x = 0; x < WIDTH; x++)
		for (y = 0; y < HEIGHT; y++)
			items[x][y] = 0;
	for (x = 0; x < WIDTH; x++)
		for (y = 0; y < HEIGHT; y++)
			less[x][y] = 0;
	for (x = 0; x < WIDTH; x++)
		for (y = 0; y < HEIGHT; y++)
			fast[x][y] = 0;
	for (x = 0; x < WIDTH; x++)
		for (y = 0; y < HEIGHT; y++)
			slow[x][y] = 0;
	Set_snake(&snake_1, 1, 3);
	Set_tail(&tail_1);
	Set_snake(&snake_2, 2, 2);
	Set_tail(&tail_2);

	for (int i = 0; i < 3; i++) {
		potal.in[i].X = 0;
		potal.in[i].Y = 0;
		potal.out[i].X = 0;
		potal.out[i].Y = 0;
	}

	p1_frame_sync = 10;
	p2_frame_sync = 10;
	iteminterval = 4;
	lessinterval = 12;
	fastinterval = 8;
	slowinterval = 10;
	time_out = 60;
//	time_out = 99;
	keep_moving = 1;
	cls(BLACK, WHITE);
	removeCursor();
	sprintf(cmd, "mode con cols=%d lines=%d", 160, 45);
	system(cmd);
}

void show_time(int remain_time)
{
	textcolor(WHITE, BLACK);
	gotoxy(WIDTH / 2 - 10, 0);
	printf("남은시간 : %02d", remain_time);

}
void show_menu() {
	textcolor(GRAY2, BLACK);
	gotoxy(134, 6);
	printf("<Player Key>");
	textcolor(BLUE2, BLACK);
	gotoxy(130, 8);
	printf("Player1 : ← ↑ → ↓");
	textcolor(RED2, BLACK);
	gotoxy(130, 10);
	printf("Player2 :  w  a  s  d");
	textcolor(YELLOW2, BLACK);
	
	textcolor(GRAY2, BLACK);
	gotoxy(135, 14);
	printf("<Item>");
	gotoxy(125, 16);
	textcolor(YELLOW2, BLACK);
	printf("★ : 꼬리 2개 증가(score +50)");
	textcolor(RED2, BLACK);
	gotoxy(125, 18);
	printf("※ : 꼬리 개수 속도 초기화");
	textcolor(GREEN2, BLACK);
	gotoxy(125, 20);
	printf("≫ : 속도 증가(score -20)");
	textcolor(MAGENTA2, BLACK);
	gotoxy(125, 22);
	printf("≪ : 속도 감소(score +100)");
	textcolor(CYAN2, BLACK);
	gotoxy(125, 24);
	printf("▒ : 포탈");
	textcolor(WHITE, BLACK);
	gotoxy(125, 26);
	printf("□ : 벽 (부딪히면 생명 1 감소)");


	gotoxy(28, 35);
	textcolor(GRAY2, BLACK);
	printf("<Victory>");
	gotoxy(17, 37);
	printf("게임이 끝날때 점수가 더 높은사람");
	gotoxy(17, 39);
	printf("점수가 같을시 생명이 더 많은사람");

	gotoxy(89, 35);
	printf("<Lose>");
	gotoxy(82, 37);
	printf("라이프가 0이 되면 패배");
	gotoxy(82, 39);
	printf("상대방에게 닿으면 패배");

	textcolor(WHITE, BLACK);
}
void first_menu() {

	char check;
	char cmd[100];
	int c1;
	removeCursor();
	sprintf(cmd, "mode con cols=%d lines=%d", 125, 45);
	system(cmd);


	while (1) {
		c1 = rand() % 16;
		textcolor(c1, BLACK);

		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				gotoxy(x * 2 + 4, y + 4);
				if (map[y][x] == 1)
					printf("■");
			}
		}
		c1 = rand() % 2 + 7;
		gotoxy(55, 38);
		textcolor(c1, BLACK);
		printf("PRESS ANY KEY");
		textcolor(WHITE, BLACK);

		if (kbhit() == 1) { check = getch(); break; }
	}

	cls(BLACK, WHITE);
}

void player1(unsigned char ch)
{
	static int oldx = 60, oldy = 10, newx = 60, newy = 10;
	int move_flag = 0;
	static unsigned char last_ch = 0;
	int i;


	if (called[0] == 0) { // 처음 또는 Restart
		oldx = 60, oldy = 10, newx = 60, newy = 10;
		Set_snake(&snake_1, 1, 3);
		Set_tail(&tail_1);
		for (i = 0; i < SNAKE_LENGTH; i++) {
			snake_1.snake[i].X = oldx - i * 2;
			snake_1.snake[i].Y = oldy;
		}
		removeCursor();
		show_snake(&snake_1, &tail_1);
		called[0] = 1;
		last_ch = RIGHT;
	}

	switch (ch) {
	case UP:
		tail_1.x[tail_1.rear] = newx;
		tail_1.y[tail_1.rear] = newy;
		tail_1.dir[tail_1.rear] = 0;
		tail_1.rear++;
		break;
	case DOWN:
		tail_1.x[tail_1.rear] = newx;
		tail_1.y[tail_1.rear] = newy;
		tail_1.dir[tail_1.rear] = 1;
		tail_1.rear++;
		break;
	case LEFT:
		tail_1.x[tail_1.rear] = newx;
		tail_1.y[tail_1.rear] = newy;
		tail_1.dir[tail_1.rear] = 2;
		tail_1.rear++;
		break;
	case RIGHT:
		tail_1.x[tail_1.rear] = newx;
		tail_1.y[tail_1.rear] = newy;
		tail_1.dir[tail_1.rear] = 3;
		tail_1.rear++;
		break;
	}

	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 3) // 0 은 Status Line
			newy = oldy - 1;
		else { // 벽에 부딛치면 방향을 반대로 이동
			newy = oldy + 1;
			last_ch = DOWN;
			tail_1.x[tail_1.rear] = newx;
			tail_1.y[tail_1.rear] = newy;
			tail_1.dir[tail_1.rear] = 1;
			tail_1.rear++;
			snake_1.life--;
		}

		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 3)
			newy = oldy + 1;
		else {
			newy = oldy - 1;
			last_ch = UP;
			tail_1.x[tail_1.rear] = newx;
			tail_1.y[tail_1.rear] = newy;
			tail_1.dir[tail_1.rear] = 0;
			tail_1.rear++;
			snake_1.life--;
		}

		move_flag = 1;
		break;
	case LEFT:
		if (oldx > 2)
			newx = oldx - 2;
		else {
			newx = oldx + 2;
			last_ch = RIGHT;
			tail_1.x[tail_1.rear] = newx;
			tail_1.y[tail_1.rear] = newy;
			tail_1.dir[tail_1.rear] = 3;
			tail_1.rear++;
			snake_1.life--;
		}

		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 4)
			newx = oldx + 2;
		else {
			newx = oldx - 2;
			last_ch = LEFT;
			tail_1.x[tail_1.rear] = newx;
			tail_1.y[tail_1.rear] = newy;
			tail_1.dir[tail_1.rear] = 2;
			tail_1.rear++;
			snake_1.life--;
		}

		move_flag = 1;
		break;
	}

	if (move_flag) {

		if (items[newx][newy])
		{
			snake_1.length += 2;
			items[newx][newy] = 0;
			item_count--;
			snake_1.score += 50;
			
		}

		if (less[newx][newy])
		{
			less[newx][newy] = 0;

			if (snake_1.length == SNAKE_LENGTH) {}
			else {

				for (int j = 0; j < snake_1.length; j++) {
					gotoxy2(snake_1.snake[j]);
					printf("  ");
				}
				snake_1.length = SNAKE_LENGTH;
			}
			p1_frame_sync = 10;
			snake_1.dir = tail_1.dir[--tail_1.rear];
			Set_tail(&tail_1);
			less_count--;
		}

		if (fast[newx][newy])
		{
			fast[newx][newy] = 0;
			fast_count--;
			p1_frame_sync -= 2;
			snake_1.score -= 20;
		}

		if (slow[newx][newy])
		{
			slow[newx][newy] = 0;
			slow_count--;
			p1_frame_sync += 5;
			snake_1.score += 100;
		}

		show_score(&snake_1);
		show_life(&snake_1);

		if (snake_1.life == 0)
			winner = 2;

		for (int j = 0; j < 3; j++) {
			if (potal.in[j].X == newx && potal.in[j].Y == newy)
				move_potal(&newx, &newy, tail_1.dir[tail_1.rear - 1], 1);
			if (potal.out[j].X == newx && potal.out[j].Y == newy)
				move_potal(&newx, &newy, tail_1.dir[tail_1.rear - 1], 2);
		}

		if (move_snake(newx, newy, &snake_1, &tail_1, &snake_2) == 0)
			winner = 2;

		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;

	}

	if (tail_1.rear == 10)
		tail_1.rear = 0;

}

void player2(unsigned char ch)
{
	static int oldx2 = 20, oldy2 = 10, newx2 = 20, newy2 = 10;
	int move_flag = 0;
	static unsigned char last_ch2 = 0;
	int i;


	if (called[1] == 0) { // 처음 또는 Restart
		oldx2 = 20, oldy2 = 10, newx2 = 20, newy2 = 10;
		Set_snake(&snake_2, 2, 2);
		Set_tail(&tail_2);

		for (i = 0; i < SNAKE_LENGTH; i++) {
			snake_2.snake[i].X = oldx2 + i * 2;
			snake_2.snake[i].Y = oldy2;
		}
		removeCursor();
		show_snake(&snake_2, &tail_2);
		called[1] = 1;
		last_ch2 = LEFT2;
	}

	switch (ch) {
	case UP2:
		tail_2.x[tail_2.rear] = newx2;
		tail_2.y[tail_2.rear] = newy2;
		tail_2.dir[tail_2.rear] = 0;
		tail_2.rear++;
		break;
	case DOWN2:
		tail_2.x[tail_2.rear] = newx2;
		tail_2.y[tail_2.rear] = newy2;
		tail_2.dir[tail_2.rear] = 1;
		tail_2.rear++;
		break;
	case LEFT2:
		tail_2.x[tail_2.rear] = newx2;
		tail_2.y[tail_2.rear] = newy2;
		tail_2.dir[tail_2.rear] = 2;
		tail_2.rear++;
		break;
	case RIGHT2:
		tail_2.x[tail_2.rear] = newx2;
		tail_2.y[tail_2.rear] = newy2;
		tail_2.dir[tail_2.rear] = 3;
		tail_2.rear++;
		break;
	}


	if (keep_moving && ch == 0)
		ch = last_ch2;
	last_ch2 = ch;

	switch (ch) {
	case UP2:
		if (oldy2 > 3) // 0 은 Status Line
			newy2 = oldy2 - 1;
		else { // 벽에 부딛치면 방향을 반대로 이동
			newy2 = oldy2 + 1;
			last_ch2 = DOWN2;
			tail_2.x[tail_2.rear] = newx2;
			tail_2.y[tail_2.rear] = newy2;
			tail_2.dir[tail_2.rear] = 1;
			tail_2.rear++;
			snake_2.life--;
		}
		move_flag = 1;
		break;
	case DOWN2:
		if (oldy2 < HEIGHT - 3)
			newy2 = oldy2 + 1;
		else {
			newy2 = oldy2 - 1;
			last_ch2 = UP2;
			tail_2.x[tail_2.rear] = newx2;
			tail_2.y[tail_2.rear] = newy2;
			tail_2.dir[tail_2.rear] = 0;
			tail_2.rear++;
			snake_2.life--;
		}
		move_flag = 1;
		break;
	case LEFT2:
		if (oldx2 > 2)
			newx2 = oldx2 - 2;
		else {
			newx2 = oldx2 + 2;
			last_ch2 = RIGHT2;
			tail_2.x[tail_2.rear] = newx2;
			tail_2.y[tail_2.rear] = newy2;
			tail_2.dir[tail_2.rear] = 3;
			tail_2.rear++;
			snake_2.life--;
		}
		move_flag = 1;
		break;
	case RIGHT2:
		if (oldx2 < WIDTH - 4)
			newx2 = oldx2 + 2;
		else {
			newx2 = oldx2 - 2;
			last_ch2 = LEFT2;
			tail_2.x[tail_2.rear] = newx2;
			tail_2.y[tail_2.rear] = newy2;
			tail_2.dir[tail_2.rear] = 2;
			tail_2.rear++;
			snake_2.life--;
		}
		move_flag = 1;
		break;
	}



	//생명감소막음
	//snake_2.life = 3;

	if (move_flag) {

		if (items[newx2][newy2])
		{
			snake_2.length += 2;
			items[newx2][newy2] = 0;
			item_count--;
			
		}


		if (less[newx2][newy2])
		{
			less[newx2][newy2] = 0;

			if (snake_2.length == SNAKE_LENGTH) {}
			else {

				for (int j = 0; j < snake_2.length; j++) {
					gotoxy2(snake_2.snake[j]);
					printf("  ");
				}
				snake_2.length = SNAKE_LENGTH;
			}
			p2_frame_sync = 10;
			snake_2.dir = tail_2.dir[--tail_2.rear];
			Set_tail(&tail_2);
		}

		if (fast[newx2][newy2])
		{
			fast[newx2][newy2] = 0;
			fast_count--;
			p2_frame_sync--;
			snake_2.score -= 20;
		}

		if (slow[newx2][newy2])
		{
			slow[newx2][newy2] = 0;
			slow_count--;
			p2_frame_sync += 5;
			snake_2.score += 100;
		}
		
		show_score(&snake_2);
		show_life(&snake_2);

		if (snake_2.life == 0)
			winner = 1;

		for (int j = 0; j < 3; j++) {
			if (potal.in[j].X == newx2 && potal.in[j].Y == newy2)
				move_potal(&newx2, &newy2, tail_2.dir[tail_2.rear - 1], 1);
			if (potal.out[j].X == newx2 && potal.out[j].Y == newy2)
				move_potal(&newx2, &newy2, tail_2.dir[tail_2.rear - 1], 2);
		}

		if (move_snake(newx2, newy2, &snake_2, &tail_2, &snake_1) == 0)
			winner = 1;
		oldx2 = newx2; // 마지막 위치를 기억한다.
		oldy2 = newy2;

	}

	if (tail_2.rear == 10)
		tail_2.rear = 0;

}

typedef struct {
	int run;
	int start;
	int remain;
	int last;
	int item;
	int less;
	int fast;
	int slow;
	int potal;
}TIME;

void main()
{
	cls(BLACK, WHITE);
	// 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	unsigned char ch;
	int c1, c2;
	TIME t;

	first_menu();

START:

	init_game();
	show_menu();
	t.potal = t.fast = t.slow = t.less = t.item = 0;
	t.start = time(NULL);
	t.last = t.remain = time_out;


	Sleep(Delay);

	show_score(&snake_1);
	show_score(&snake_2);
	show_life(&snake_1);
	show_life(&snake_2);
	show_time(t.remain);
	draw_box2(0, 2, WIDTH - 2, HEIGHT - 2, "□");



	while (1) {
		// 시간 check

		if (winner != 0)
			break;

		t.run = time(NULL) - t.start;

		if (t.run > t.item && (t.run % iteminterval == 0)) { // ITEM
			show_item();
			t.item = t.run;
		}
		if (t.run > t.less && (t.run % lessinterval == 0)) { // LESS
			show_less();
			t.less = t.run;
		}
		if (t.run > t.fast && (t.run % fastinterval == 0)) { //SPEED
			show_fast();
			t.fast = t.run;
		}
		if (t.run > t.slow && (t.run % slowinterval == 0)) { //SPEED
			show_slow();
			t.slow = t.run;
		}

		if (t.run > t.potal && (t.run % potalinterval == 0)) { //SPEED
			make_potal();
			t.potal = t.run;
		}

		t.remain = time_out - t.run;
		if (t.remain < t.last) {
			show_time(t.remain); // 시간이 변할때만 출력
			t.last = t.remain;
		}

		if (t.remain == 0) {

			if (snake_1.score > snake_2.score)
				winner = 1;
			else if (snake_1.score < snake_2.score)
				winner = 2;
			else {
				if (snake_1.life > snake_2.life)
					winner = 1;
				else if (snake_1.life < snake_2.life)
					winner = 2;
			}
			break;
		}


		if (kbhit() == 1) {  // 키보드가 눌려져 있으면
			ch = getch(); // key 값을 읽는다
			// ESC 누르면 프로그램 종료

			if (ch == ESC) {
				//gotoxy(25,11);
				//exit(0);
				break;
			}
			if (ch == SPECIAL1 || ch == SPECIAL2) { // 만약 특수키
				// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
				ch = getch();
				// Player1은 방향키로 움직인다.
				switch (ch) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					player1(ch);//player1만 방향 전환
					if (frame_count % p2_frame_sync == 0)
						player2(0);
					break;
				default: // 방향 전환이 아니면
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					if (frame_count % p2_frame_sync == 0)
						player2(0);
				}
			}
			else {
				// Player2은 AWSD 로 움직인다.
				switch (ch) {
				case UP2:
				case DOWN2:
				case LEFT2:
				case RIGHT2://player2만 방향 전환
					player2(ch);
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					break;
				default:// 방향 전환이 아니면
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					if (frame_count % p2_frame_sync == 0)
						player2(0);
				}
			}
		}
		else {
			// keyboard 가 눌려지지 않으면 계속 움직인다.
			// 이동중이던 방향으로 계속 이동
			if (frame_count % p1_frame_sync == 0)
				player1(0);
			if (frame_count % p2_frame_sync == 0)
				player2(0);
		}
		if (frame_count % item_frame_sync == 0)
			move_item();
		if (frame_count % less_frame_sync == 0)
			move_less();
		if (frame_count % fast_frame_sync == 0)
			move_fast();
		if (frame_count % slow_frame_sync == 0)
			move_slow();

		show_potal();

		Sleep(10); // Delay 값을 줄이고
		frame_count++; // frame_count 값으로 속도 조절을 한다.
	}


	gotoxy(WIDTH / 2 - 13, HEIGHT / 3);
	
	textcolor(BLACK, GREEN2);
	if (winner == 1) {
		printf(" WINNER IS ");
		textcolor(BLUE2, GREEN2);
		printf("PLAYER1!! ");
	}
	else if (winner == 2) {
		printf(" WINNER IS ");
		textcolor(RED2,GREEN2);
		printf("PLAYER2!! ");
	}
	else {
		printf("       DRAW!!        ");
	}
	while (1) {

		do { // 색을 변경하면서 Game Over 출력
			c1 = rand() % 16;
			c2 = rand() % 16;
		} while (c1 == c2);
		textcolor(c1, c2);
		gotoxy(WIDTH / 2 - 10, HEIGHT / 3 + 3);
		printf("** Game Over **");
		gotoxy(WIDTH / 2 - 18, HEIGHT / 3 + 6);
		textcolor(BLACK, WHITE);
		printf("Hit (R) to Restart (Q) to Quit");
		Sleep(Delay);
		if (kbhit()) {
			ch = getch();
			if (ch == 'r' || ch == 'q')
				break;
		}
	}
	if (ch == 'r')
		goto START;
	cls(BLACK, WHITE);
	gotoxy(WIDTH / 2 - 18, HEIGHT / 3 + 6);
}