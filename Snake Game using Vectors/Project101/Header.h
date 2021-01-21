#pragma once

class SnakePlayer //class for snake
{
public:
	SnakePlayer() {}; //default constructor
	SnakePlayer(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	int x;
	int y;
	//function declartions
	void DisplayBoard();
	void InitializeBoard();
	void SnakeInit();
	void Food();
	void Move();
	void DeleteLastSnake();
	bool ateFood();
	bool Won();
	bool Lost();
	void displaymenu();
};
