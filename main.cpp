#include "vector2.h"

#include <cmath>
#include <conio.h>
#include <ctime>
#include <stdio.h>
#include <vector>
#include <windows.h>

#define MAX_MAP_WIDTH 17
#define MAX_MAP_HEIGHT 17

using namespace std;

/*
 * -32 72	- Up		- 0
 * -32 75	- Left		- 1
 * -32 77	- Right 	- 2
 * -32 80	- Down		- 3
 */
HANDLE hConsole;
bool terminateGame = false;

char buffer[MAX_MAP_HEIGHT * MAX_MAP_WIDTH * 4 + MAX_MAP_HEIGHT] = {0}; // MAX_MAP_HEIGHT * MAX_MAP_WIDTH + MAX_MAP_HEIGHT

char input;
vector<Vector2> snake;
Vector2 snakeDirection = Vector2::right;
int points = 0;

const char renderChars[] = {' ', 'O', '*', 'P', 'V', '^', '>', '<'};

vector<Vector2> eatable;

bool MoveHandler();
bool InputHandler();
void DrawScreen();
void PrintInfo();
void eat(int eatableIndex);
Vector2 randomPos();

int main() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL));
	snake.push_back(Vector2(-5, 0));
	snake.push_back(Vector2(-6, 0));
	eatable.push_back(randomPos());
	while (input != 3 && !terminateGame) {
		// Debug vectors
		/*for (int i = 0; i < snake.size(); i++) {
			printf("[%i]%s\r\n", i, snake[i].ToString().c_str());
		}*/
		DrawScreen();
		Sleep(250);
		InputHandler();
		if (!MoveHandler()) terminateGame = true;
	}
	return 0;
}

bool MoveHandler() {
	for (int i = snake.size() - 1; i >= 1; i--) {
		snake[i] = snake[i - 1];
	}
	snake[0] = snake[0] + snakeDirection;

	// Kill on border
	if (abs(snake[0].x) > floor(MAX_MAP_WIDTH / 2) + (MAX_MAP_WIDTH % 2 != 0 ? 0 : -1)) return false;
	if (abs(snake[0].y) > floor(MAX_MAP_HEIGHT / 2) + (MAX_MAP_HEIGHT % 2 != 0 ? 0 : -1)) return false;

	// Eatables
	for (int i = 0; i < eatable.size(); i++) {
		if (eatable[i] == snake[0]) {
			eat(i);
			return true;
		}
	}

	// Snake hit
	for (int i = snake.size() - 1; i > 0; i--) {
		if (snake[0] == snake[i]) {
			printf("[0] %s\r\n[%i] %s\r\n", snake[0].ToString().c_str(), i, snake[i].ToString().c_str());
			printf("%s", (snake[0] == snake[i] ? "true" : "false"));
			return false;
		}
	}

	return true;
}
bool InputHandler() {
	if (!_kbhit()) return false;

	input = _getch();
	if (input == 3) {
		terminateGame = true;
		return true;
	}
	if (input < 0) {
		input = _getch();
		if (input == 72 && !snakeDirection.Equals(Vector2::down)) snakeDirection = Vector2::up;
		else if (input == 80 && !snakeDirection.Equals(Vector2::up)) snakeDirection = Vector2::down;
		else if (input == 75 && !snakeDirection.Equals(Vector2::right)) snakeDirection = Vector2::left;
		else if (input == 77 && !snakeDirection.Equals(Vector2::left)) snakeDirection = Vector2::right;
		return true;
	}
	return false;
}
void DrawScreen() {
	short renderer[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {0};

	/*int midY = ;
	int midX = ;*/
	Vector2 pos = Vector2::zero;
	Vector2 middle = Vector2(floor(MAX_MAP_WIDTH / 2) + (MAX_MAP_WIDTH % 2 != 0 ? 0 : -1), floor(MAX_MAP_HEIGHT / 2) + (MAX_MAP_HEIGHT % 2 != 0 ? 0 : -1));
	renderer[(int)middle.y][(int)middle.x] = 1;

	// Put snake into renderer
	for (int i = 0; i < snake.size(); i++) {
		Vector2 yRev = Vector2(snake[i].x, snake[i].y * -1);
		pos = middle + yRev;
		if (i == 0) {
			renderer[(int)pos.y][(int)pos.x] = (snakeDirection.x > 0 ? 7 : snakeDirection.x < 0 ? 6
																	   : snakeDirection.y > 0	? 4
																								: 5);
		} else {
			renderer[(int)pos.y][(int)pos.x] = 2;
		}
	}

	// Put eatable into renderer
	for (int i = 0; i < eatable.size(); i++) {
		Vector2 yRev = Vector2(eatable[i].x, eatable[i].y * -1);
		pos = middle + yRev;
		renderer[(int)pos.y][(int)pos.x] = 3;
	}

	// Print Buffer
	int bptr = 0;
	for (int i = 0; i < MAX_MAP_HEIGHT; i++) {
		for (int j = 0; j < MAX_MAP_WIDTH; j++) {
			// printf("|%c|", renderChars[renderer[i][j]]);
			buffer[bptr] = '|';
			buffer[bptr + 1] = renderChars[renderer[i][j]];
			buffer[bptr + 2] = '|';
			bptr += 3;
		}
		// printf("\r\n");
		buffer[bptr] = '\r';
		buffer[bptr + 1] = '\n';
		bptr += 2;
	}
	system("cls");
	PrintInfo();
	setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
	printf("|%s|", buffer);
	memset(buffer, 0, MAX_MAP_HEIGHT * MAX_MAP_WIDTH * 4 + MAX_MAP_HEIGHT);
}

void PrintInfo() {
	printf("  Points: %10i\r\n", points);
}

void eat(int eatableIndex) {
	Vector2 newDirection = snake[snake.size() - 1] - snake[snake.size() - 2];
	snake.push_back(snake[snake.size() - 1] + newDirection);
	eatable.erase(eatable.begin() + eatableIndex);
	eatable.push_back(randomPos());
	points++;
}

Vector2 randomPos() {
	int x = rand() % MAX_MAP_WIDTH - floor(MAX_MAP_WIDTH / 2) + (MAX_MAP_WIDTH % 2 != 0 ? 0 : 1);
	int y = rand() % MAX_MAP_HEIGHT - floor(MAX_MAP_HEIGHT / 2) + (MAX_MAP_HEIGHT % 2 != 0 ? 0 : 1);
	return Vector2(x, y);
}