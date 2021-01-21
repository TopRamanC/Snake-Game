#include <conio.h>
#include <stdio.h> 
#include <ctime> //for time
#include <vector> //to increment snake
#include <windows.h> //gotoxy
#include <algorithm> //for vector
#include <iostream>
#include <string>
#include "Header.h"
using namespace std;

const int height = 20;
const int width = 50;
const int startLength = 3;
const int FPS = 5;

int snakeLength = startLength;
char Direction = 'd'; //snake will start in the right direction
int foodX;
int foodY;
char board[height * width];

vector < SnakePlayer > SnakeVector; //declaring out vector

void ShowConsoleCursor(bool showFlag) //to hide the cursor
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; //hide cursor
	SetConsoleCursorInfo(out, &cursorInfo);
}

void gotoxy(int column, int line) //gotoxy
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

void SnakePlayer::DisplayBoard() //display the board
{
	for (int i = 0; i < width + 2; i++) //top of the board
	{
		cout << char(219);
	}
	cout << endl;

	for (int y = 0; y < height; y++)
	{
		cout << char(178); //left bound
		for (int x = 0; x < width; x++)
		{
			cout << board[y * width + x]; //stuff: X = food, O     = snake segment, ' ' = blank space
		}
		printf("%c\n", char(178)); //right bound
	}

	for (int i = 0; i < width + 2; i++) //bottom of the board
	{
		printf("%c", char(219));
	}
	cout << endl;
	cout << "Snake size: " << snakeLength;

}

void SnakePlayer::InitializeBoard() //intialize the whole board to ' '
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			board[y * width + x] = ' '; //sets all plane cells to ' ' 
		}
	}
}

void SnakePlayer::SnakeInit() //spawns the snake in the board with the length of 3
{
	for (int i = 0; i < snakeLength; i++)
	{
		SnakePlayer snake;
		snake.x = i;
		snake.y = 0;
		SnakeVector.push_back(snake); //push back first location
		board[i] = 'O'; //display a 'O'
	}
}

void SnakePlayer::Food() //to generate food randomly on our board
{
	foodX = rand() % width;
	foodY = rand() % height;

	while (board[foodY * width + foodX] != ' ') //new food will spawn only on a empty spot
	{
		foodX = rand() % width;
		foodY = rand() % height;
	}

	board[foodY * width + foodX] = 'X'; //'X' is the symbol for food
}

void SnakePlayer::Move() //to move the snake when user inputs direction
{
	if (Direction == 'd') //go right
	{
		SnakePlayer snake = SnakeVector.at(SnakeVector.size() - 1); //send the size as the parameter
		snake.x++; //move snake to the right
		SnakeVector.push_back(snake); //push 'snake' into our vector
		board[snake.y * width + snake.x] = 'o'; //assign 'o'

		DeleteLastSnake(); //delete the old snake
	}
	else if (Direction == 's') //go down
	{
		SnakePlayer snake = SnakeVector.at(SnakeVector.size() - 1);//send the size as the parameter
		snake.y++; //move snake up
		SnakeVector.push_back(snake); //push 'snake' into our vector
		board[snake.y * width + snake.x] = 'o'; //assign 'o'

		DeleteLastSnake(); //delete old snake
	}
	else if (Direction == 'a') //go left
	{
		SnakePlayer snake = SnakeVector.at(SnakeVector.size() - 1);//send the size as the parameter
		snake.x--; //move snake left
		SnakeVector.push_back(snake); //push 'snake' into our vector
		board[snake.y * width + snake.x] = 'o'; //assign 'o'

		DeleteLastSnake(); //delete last snake
	}
	else if (Direction == 'w') //go up
	{
		SnakePlayer snake = SnakeVector.at(SnakeVector.size() - 1);//send the size as the parameter
		snake.y--; //move snake down
		SnakeVector.push_back(snake); //push 'snake' into our vector
		board[snake.y * width + snake.x] = 'o'; //assign '0'

		DeleteLastSnake(); //delete last snake
	}
}

void SnakePlayer::DeleteLastSnake() //delete last snake
{
	SnakePlayer temp = SnakeVector[0];
	board[temp.y * width + temp.x] = ' ';
	SnakeVector.erase(SnakeVector.begin()); //first is last
}

bool SnakePlayer::ateFood()
{
	SnakePlayer snake = SnakeVector.at(SnakeVector.size() - 1); //first element of snake
	SnakePlayer food;
	food.x = foodX;
	food.y = foodY;
	return snake.x == food.x && snake.y == food.y; //if his "head" collides with food cell
}

bool SnakePlayer::Won() //if your snake gets too big and none of the cells are empty you win
{
	bool space = false; //intiliazed to false until the user wins
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (board[y * width + x] == ' ') //if none of cells is empty we won
			{
				space = true;
				break;
			}
		}
	}
	return !space;
}

bool SnakePlayer::Lost() //check if user lost
{
	SnakePlayer snake = SnakeVector.at(SnakeVector.size() - 1); //first element of snake

	if (snake.y < 0 || snake.y > height - 1) //touching the top or bottom boudnary will kill you
	{
		return true;
	}

	//making portals with the left and right boundaries
	if (snake.x >= width - 1)  //right side
	{
		snake.x = 0; //change the x value back to 0
	}
	else if (snake.x < 0) //left side
	{
		snake.x = width - 1; //change x value to the right side
	}

	for (int i = 0; i < SnakeVector.size() - 1; i++) //if the snake hits itself then the user loses
	{
		SnakePlayer temp = SnakeVector[i];
		if (temp.x == snake.x && temp.y == snake.y) //return true if snake hits itself
		{
			return true;
		}
	}
	return false;
}

void SnakePlayer::displaymenu() //display menu and picture art
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	cout << "0 - BLACK        " << "                          " << "               /||\\ " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	cout << "1 - BLUE         " << "                         " << "               |||| " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	cout << "2 - GREEN        " << "                         " << "               ||||                      _____.-..-. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	cout << "3 - CYAN         " << "                         " << "               |||| /|\\               .-~@@/ / q  p \\ " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "4 - RED          " << "                        " << "           /|\\  |||| |||             .'@ _@/..\\-.__.-/ " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
	cout << "5 - MAGENTA      " << "                         " << "          |||  |||| |||             /@.-~/|~~~`\|__|/ " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
	cout << "6 - BROWN        " << "                         " << "          |||  |||| |||            |'--<||     '~~' " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout << "7 - LIGHTGRAY    " << "                         " << "          |||  |||| d||            |>--<\\@\\ " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "8 - DARKGRAY     " << "                         " << "          |||  |||||||/            \\>---<\\@`\\. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	cout << "9 - LIGHTBLUE    " << "                         " << "          ||b._||||~~'              `\\>---<`\\@`\\. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "10 - LIGHTGREEN  " << "                         " << "          \\||||||||                   `\\>----<`\\@`\\. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	cout << "11 - LIGHTCYAN   " << "                         " << "           `~~~||||               _     `\\>-----<`\\@`\\. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "12 - LIGHTRED    " << "                         " << "               ||||              (_)      `\\>-----<`\\.@`\\. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	cout << "13 - LIGHTMAGENTA" << "                         " << "               ||||              (_)        `\\>------<`\\.@`\\. " << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	cout << "14 - YELLOW      " << "                        " << "~~~~~~~~~~~~~~~~||||~~~~~~~~~~~~~~(__)~~~~~~~~~`\\>-------<`\\.@`\\~~~~~~~~~~~" << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "15 - WHITE       " << "                       " << "   \\/..__..--  . |||| \\/  .  ..____( _)@@@--..____\\..--\\@@@/~`\\@@>-._   \\/ . " << endl;

	cout << endl;

}

int main()
{
	SnakePlayer R;
	ShowConsoleCursor(false); //hide the cursor
	int x;

	R.displaymenu(); //display the menu for the user

	cout << "Pick the color you want: "; //ask user to pick their color
	cin >> x; //get input

	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x); //change color to user choice

	srand((unsigned int)time(NULL)); //for dispaying random food

	R.InitializeBoard(); //intitalize all the board to ' '
	R.SnakeInit(); //display the snake
	R.Food(); //display the food randomly

	while (R.Lost() != true)
	{
		if (_kbhit()) //can get keyboard input without pressing Enter key
		{
			Direction = _getch(); //get direction
		}

		if (R.ateFood()) //if the snake eats food
		{
			snakeLength++;
			SnakePlayer snake(foodX, foodY);
			SnakeVector.push_back(snake); //we add eaten food position as a new snake segment
			R.Food(); //and spawn new food
		}
		gotoxy(0, 0); //goes to (0,0) everytime to display everthing
		R.DisplayBoard(); //display board
		R.Move(); //direction for snake

		if (R.Won()) //if the user won
		{
			cout << "Congratulation, you have won!";
			break;
		}
		if (R.Lost()) //if the user lost
		{
			system("cls");

			cout << "############################################################" << endl;
			cout << endl;
			cout << " ############         ##          ##         ##   ##########" << endl;
			cout << "#                    #  #         # #       # #   #" << endl;
			cout << "#                   #    #        #  #     #  #   #" << endl;
			cout << "#  ##########      ########       #   #   #   #   ##########" << endl;
			cout << "#            #    #        #      #     ##    #   #" << endl;
			cout << "#            #   #          #     #           #   #" << endl;
			cout << " ############   #            #    #           #   ##########" << endl;
			cout << "" << endl;
			cout << "##############  #            #    #############   ##########" << endl;
			cout << "#            #   #          #     #               #        #" << endl;
			cout << "#            #    #        #      #               #        #" << endl;
			cout << "#            #     #      #       #############   ##########" << endl;
			cout << "#            #      #    #        #               #      #  " << endl;
			cout << "#            #       #  #         #               #       # " << endl;
			cout << "##############        ##          #############   #        #" << endl;
			cout << endl;
			cout << "############################################################" << endl;
			break;
		}
		Sleep(20); //slow the snake down
	}
	system("PAUSE");
}