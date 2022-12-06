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
char input;
vector<Vector2> snake;
Vector2 snakeDirection = Vector2::right;

const char renderChars[] = {' ', 'O', '='};

HANDLE hConsole;

bool MoveHandler();
void DrawScreen();

int main() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL));
	snake.push_back(Vector2(-5, 0));
	while (input != 3) {
		system("cls");
		for (int i = 0; i < snake.size(); i++) {
			printf("[%i]%s\r\n", i, snake[i].ToString().c_str());
		}
		DrawScreen();
		Sleep(250);
		do {
			input = getch();
		} while (!(input == 3 || input < 0));
		if (input < 0) input = getch();
		if (input == 80) snake.push_back(Vector2(-5, 0));
		if (!MoveHandler()) input = 3;
	}
	return 0;
}

bool MoveHandler() {
	for (int i = snake.size() - 1; i >= 1; i--) {
		snake[i] = snake[i - 1];
	}
	snake[0] = snake[0] + snakeDirection;
	if (abs(snake[0].x) > floor(MAX_MAP_WIDTH / 2) + (MAX_MAP_WIDTH % 2 != 0 ? 0 : -1)) return false;
	return true;
}
void DrawScreen() {
	short buffer[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {0};

	/*int midY = ;
	int midX = ;*/
	Vector2 pos = Vector2::zero;
	Vector2 middle = Vector2(floor(MAX_MAP_WIDTH / 2) + (MAX_MAP_WIDTH % 2 != 0 ? 0 : -1), floor(MAX_MAP_HEIGHT / 2) + (MAX_MAP_HEIGHT % 2 != 0 ? 0 : -1));
	buffer[(int)middle.y][(int)middle.x] = 1;

	// Render snake into buffer
	for (int i = 0; i < snake.size(); i++) {
		pos = middle + snake[i];
		buffer[(int)pos.y][(int)pos.x] = 2;
	}

	// Print Buffer
	for (int i = 0; i < MAX_MAP_HEIGHT; i++) {
		for (int j = 0; j < MAX_MAP_WIDTH; j++) {
			printf("|%c|", renderChars[buffer[i][j]]);
		}
		printf("\r\n");
	}
}