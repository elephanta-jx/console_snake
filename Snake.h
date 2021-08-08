#pragma once
#pragma comment (lib, "User32.lib")
#include <iostream>
#include <Windows.h>
#include <list>

using namespace std;

enum DIRECTIONS
{
	RIGHT, LEFT, UP, DOWN
};


class Snake
{
public:

	Snake();

	// console functions
	void welcomeInfo();
	void gotoxy(POINT point);
	void drawMap();
	void cleanScreen();
	void start();

	// snake functions
	bool move(DIRECTIONS direction);
	bool move();
	void generateFood();
	void flashSnake();
	
private:
	int length;
	bool dead;
	DIRECTIONS direction;
	list<POINT> position;

	HANDLE hConsoleOut = NULL;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo = { 0 };
	POINT mapStart;
	POINT mapEnd;
	POINT food = { 0 };
	int intervalTime = 0;		// level
};

