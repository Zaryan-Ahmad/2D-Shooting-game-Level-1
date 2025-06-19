#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<thread>
using namespace std;

int score = 0;
int life = 3;
int totalEnemies = 0;

const int MAX_ROWS = 100;
const int MAX_COLS = 100;

int playerRow = 28; //Player's Row
int playerCol = 9; //Player's Column

const int MAX_BULLETS = 100;
int bulletCol[MAX_BULLETS];//Column Position
int bulletRow[MAX_BULLETS];//Row Position
bool bulletActive[MAX_BULLETS] = { false };//Active State

const int MAX_ENEMIES = 10;
int enemyBulletTimer = 0;
int enemyMoveTimer = 0;
int enemyRow[MAX_ENEMIES];
int enemyCol[MAX_ENEMIES];
bool enemyAlive[MAX_ENEMIES] = { true };
int enemyDirection = 1;

const int MAX_ENEMY_BULLETS = 100;
int enemyBulletRow[MAX_ENEMY_BULLETS];
int enemyBulletCol[MAX_ENEMY_BULLETS];
bool enemyBulletActive[MAX_ENEMY_BULLETS] = { false };

char boardOne[30][20] = {
	{ '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#','E',' ',' ',' ',' ','E',' ',' ',' ',' ',' ',' ',' ','E',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ','E',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ','E',' ',' ',' ',' ',' ',' ','E',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#',' ',' ',' ',' ',' ',' ',' ',' ','P',' ',' ',' ',' ',' ',' ',' ',' ',' ','#' },
	{ '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#' }
};

void printBoard(char board[][20]);
void gotoxy(int x, int y);
void hideCursor();
void movePlayer();
void moveBullets();
void moveEnemies();
void moveEnemyBullets();
void showStats();

int main() {
	hideCursor();
	system("cls");
	printBoard(boardOne);
	int enemyCount = 0;
	for (int i = 0;i < 30 && enemyCount < MAX_ENEMIES;i++) {
		for (int j = 0;j < 20 && enemyCount < MAX_ENEMIES;j++) {
			if (boardOne[i][j] == 'E') {
				enemyRow[enemyCount] = i;
				enemyCol[enemyCount] = j;
				enemyAlive[enemyCount] = true;
				enemyCount++;
				totalEnemies++;
			}
		}
	}
	srand(GetTickCount());
	while (true) {
		movePlayer();
		moveBullets();
		enemyBulletTimer++;

		if (enemyBulletTimer >= 10) {
			for (int i = 0;i < enemyCount;i++) {
				if (enemyAlive[i]&&(rand()%5==0)) {
					//bool fired = false;
					for (int b = 0;b < MAX_ENEMY_BULLETS;b++) {
						if (!enemyBulletActive[b]) {
							enemyBulletRow[b] = enemyRow[i] + 1;
							enemyBulletCol[b] = enemyCol[i];
							enemyBulletActive[b] = true;
							//fired = true;
							break;
						}
					}
				}
			}
			enemyBulletTimer = 0;
		}
		moveEnemyBullets();
		showStats();
		int enemiesLeft = 0;
		for (int i = 0;i < enemyCount;i++) {
			if (enemyAlive[i])
				enemiesLeft++;
		}
		if (enemiesLeft == 0) {
			gotoxy(45, 8);
			cout << "You Win! Final Score: " << score;
			break;
		}
		if (life <= 0) {
			gotoxy(45, 8);
			cout << "Game Over! Final Score: " << score;
			break;
		}
		Sleep(50);
	}
	return 0;
}

void printBoard(char board[][20]) {
	for (int i = 0;i < 30;i++) {
		for (int j = 0;j < 20;j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x, coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void movePlayer() {
	if (_kbhit()) {
		char ch = _getch();
		if (ch == -32) { //Specific key prefix
			ch = _getch(); //Ask Input key
			if (ch == 75 && playerCol > 1) { //If left
				gotoxy(playerCol*2, playerRow);
				cout << " "; //Erase Old player
				playerCol--;
				gotoxy(playerCol*2, playerRow);
				cout << "P";
			}
			else if (ch == 77 && playerCol < 18) { //If right
				gotoxy(playerCol*2, playerRow);
				cout << " "; //Erase old player
				playerCol++;
				gotoxy(playerCol*2, playerRow);
				cout << "P";
			}
		}
		else if (ch == 32) {
			for (int i = 0;i < MAX_BULLETS;i++) {
				if (!bulletActive[i]) {
					bulletCol[i] = playerCol;
					bulletRow[i] = playerRow - 1;
					bulletActive[i] = true;
					break;//Only one bullet at a time
				}
			}
		}
	}
}

void moveBullets() {
	for (int i = 0;i < MAX_BULLETS;i++) {
		if (bulletActive[i]) {
			gotoxy(bulletCol[i] * 2, bulletRow[i]);
			cout << " ";
			bulletRow[i]--;
			bool hit = false;
			for (int e = 0;e < MAX_ENEMIES;e++) {
				if (enemyAlive[e] == bulletRow[i] && enemyCol[e] == bulletCol[i]) {
					enemyAlive[e] = false;
					score += 5;
					hit = true;
					gotoxy(enemyCol[e] * 2, enemyRow[e]);
					cout << " ";
					break;
				}
			}
			if (bulletRow[i] < 1||hit) {
				bulletActive[i] = false;
			}
			else {
				gotoxy(bulletCol[i] * 2, bulletRow[i]);
				cout << "^";
			}
		}
	}
}

void moveEnemies() {
	for (int i = 0;i < MAX_ENEMIES;i++) {
		if (enemyAlive[i]) {
			gotoxy(enemyCol[i] * 2, enemyRow[i]);
			cout << " ";
		}
	}
	bool reverse = false;
	for (int i = 0;i < MAX_ENEMIES;i++) {
		if (enemyAlive[i]) {
			int newCol = enemyCol[i] + enemyDirection;
			if (newCol <= 1 || newCol >= 18) {
				reverse = true;
				break;
			}
		}
	}
	if (reverse) {
		enemyDirection *= -1;
		for (int i = 0;i < MAX_ENEMIES;i++) {
			if (enemyAlive[i]) {
				enemyRow[i]++;
			}
		}
	}
	else {
		for (int i = 0;i < MAX_ENEMIES;i++) {
			if (enemyAlive[i]) {
				enemyCol[i] += enemyDirection;
			}
		}
	}
	for (int i = 0;i < MAX_ENEMIES;i++) {
		if (enemyAlive[i]) {
			gotoxy(enemyCol[i] * 2, enemyRow[i]);
			cout << "E";
		}
	}
}

void moveEnemyBullets() {
	for (int i = 0;i < MAX_ENEMY_BULLETS;i++) {
		if (enemyBulletActive[i]) {
			gotoxy(enemyBulletCol[i] * 2, enemyBulletRow[i]);
			cout << " ";
			enemyBulletRow[i]++;
			if (enemyBulletRow[i] >= 29) {
				enemyBulletActive[i] = false;
			}
			else if (enemyBulletRow[i] == playerRow && enemyBulletCol[i] == playerCol) {
				life--;
				enemyBulletActive[i] = false;
				gotoxy(playerCol * 2, playerRow);
				cout << "X";
				Sleep(100);
				gotoxy(playerCol * 2, playerRow);
				cout << "P";
			}
			else {
				gotoxy(enemyBulletCol[i] * 2, enemyBulletRow[i]);
				cout << "v";
			}
		}
	}
}

void showStats() {
	gotoxy(45, 5);
	cout << "Score: " << score << "   ";
	gotoxy(45, 6);
	cout << "Life: " << life << "   ";
}