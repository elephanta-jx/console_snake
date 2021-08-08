/*	
	Game: Snake
	Author: Jack Xiang
	Version: 1.3
*/

#include "Snake.h"
#include <ctime>


int main(int argc, char* argv[])
{
	srand(time(0));

	Snake snake;
	snake.start();

	return 0;
}

