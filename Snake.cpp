#include "Snake.h"

Snake::Snake()
{
	// get console handle
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);		
	if (hConsoleOut == NULL)
		exit(1);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	// map coordinates
	mapStart = { 0, 0 };
	mapEnd = { 30, 20 };
	intervalTime = 800;

	length = 5;
	direction = RIGHT;
	dead = false;

	this->welcomeInfo();
	// initiate position of the snake
	POINT temp = {mapStart.x + 1, mapStart.y + 1};
	for (int i = 0; i < length; i++, temp.x++)
	{
		position.push_front(temp);
	}

	this->drawMap();
}

void Snake::welcomeInfo()
{
	cout << "                     Snake" << endl;
	cout << "************************************************" << endl;
	cout << "version£º1.2\n";
	cout << "************************************************" << endl;
	cout << "Guide£º¡û¡ú¡ü¡ý to control the snake" << endl;
	cout << "Press enter to begin\n";

	while (!GetAsyncKeyState(0x0D))		// press enter to begin
	{
		Sleep(200);
	}
}


// move the cursor to (x, y)
void Snake::gotoxy(POINT point)
{
	csbiInfo.dwCursorPosition.X = (short)point.x * 2;		// abstract x-axis
	csbiInfo.dwCursorPosition.Y = (short)point.y;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}

void Snake::drawMap()
{
	cleanScreen();

	for (int i = 0; i < mapStart.y; i++)		// skip empty lines
		cout << endl;

	for (int i = 0; i < (mapEnd.y - mapStart.y + 1); i++)		// loop for lines
	{
		for (int j = 0; j < mapStart.x; j++)	// skip leading spaces
			cout << "  ";						// two spaces

		if (i == 0 || i == (mapEnd.y - mapStart.y))		// print border
		{
			for (int j = 0; j < mapEnd.x - mapStart.x + 1; j++)
				cout << "¡õ";
		}
		else
		{
			cout << "¡õ";
			for (int j = 0; j < mapEnd.x - mapStart.x - 1; j++)		// print spaces between 
				cout << "  ";
			cout << "¡õ";
		}

		cout << endl;
	}

	// draw snake
	list<POINT>::iterator it; 
	for (it = position.begin(); it != position.end(); it++)
	{
		gotoxy(*it);
		cout << "¡ö";
	}

	generateFood();

}


void Snake::cleanScreen()
{
	system("CLS");
}

void Snake::start()
{
	while (!(GetAsyncKeyState(0x1b)))
	{
		Sleep(intervalTime);

		if (GetAsyncKeyState(0x25))			// left key
			this->move(LEFT);
		else if (GetAsyncKeyState(0x26))	// up key
			this->move(UP);
		else if (GetAsyncKeyState(0x27))	// right key
			this->move(RIGHT);
		else if (GetAsyncKeyState(0x28))	// down key
			this->move(DOWN);
		else
			this->move();

		if (dead)
		{
			flashSnake();
			break;
		}

		switch (length)
		{
		case 7:
			intervalTime = 600;
			break;
		case 10:
			intervalTime = 400;
			break;
		case 15:
			intervalTime = 300;
			break;
		case 20:
			intervalTime = 200;
			break;
		case 30:
			intervalTime = 100;
			break;
		
		}
	}

	cleanScreen();
	cout << "Game Over" << endl;
}

bool Snake::move(DIRECTIONS direction)
{
	switch (this->direction)	// eliminate conditions that would not work
	{
	case RIGHT:
	case LEFT:
		if (direction == LEFT || direction == RIGHT)
			return false;
		else
			break;
	case UP:
	case DOWN:
		if (direction == UP || direction == DOWN)
			return false;
		else
			break;
	default:
		return false;
	}

	this->direction = direction;
	this->move();

	return true;
}

bool Snake::move()
{
	POINT nextPoint = position.front();
 
	switch (this->direction)
	{
	case RIGHT:
		nextPoint.x++;
		break;
	case LEFT:
		nextPoint.x--;
		break;
	case UP:
		nextPoint.y--;
		break;
	case DOWN:
		nextPoint.y++;
		break;
	default:
		return false;
	}

	//check if the snake hit the wall
	if (nextPoint.x <= mapStart.x || nextPoint.x >= mapEnd.x || nextPoint.y <= mapStart.y || nextPoint.y >= mapEnd.y)
	{
		dead = true;
		return false;
	}
	
	//check if the snake crash itself
	list<POINT>::iterator it = position.begin();
	for (int i = 0; i < position.size() - 1; i++, ++it)
	{
		if (nextPoint.x == (*it).x && nextPoint.y == (*it).y)
		{
			dead = true;
			return false;
		}

	}

	position.push_front(nextPoint);	// add a new head
	gotoxy(nextPoint);
	cout << "¡ö";

	//check if the snake gets the food
	if (nextPoint.x == food.x && nextPoint.y == food.y)
	{
		length++;
		generateFood();
	}	
	else
	{
		gotoxy(position.back());	// delete the tail
		cout << "  ";	
		position.pop_back();	
	}
		
	

	return true;
}

void Snake::generateFood()
{
	POINT temp;
	bool flag = true;

	while (flag)
	{
		temp.x = rand() % (mapEnd.x - mapStart.x - 1) + (mapStart.x + 1);
		temp.y = rand() % (mapEnd.y - mapStart.y - 1) + (mapStart.y + 1);

		if (temp.x == food.x && temp.y == food.y)
			continue;

		list<POINT>::iterator it;
		for (it = position.begin(); it != position.end(); it++)
		{
			if (temp.x == (*it).x && temp.y == (*it).y)
				break;
		}

		if (it == position.end())
			flag = false;
	}
	
	food = temp;
	gotoxy(food);		// move cursor to x,y

	SetConsoleTextAttribute(hConsoleOut, FOREGROUND_GREEN | FOREGROUND_BLUE);		// change text color 
	cout << "¡ö";
	SetConsoleTextAttribute(hConsoleOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);	// change text color back

}

void Snake::flashSnake()
{
	for (int i = 0; i < 3; i++)
	{
		Sleep(500);
		list<POINT>::iterator it;

		for (it = position.begin(); it != position.end(); it++)
		{
			gotoxy(*it);
			cout << "  ";
		}

		Sleep(500);
		for (it = position.begin(); it != position.end(); it++)
		{
			gotoxy(*it);
			cout << "¡ö";
		}

	}
}


