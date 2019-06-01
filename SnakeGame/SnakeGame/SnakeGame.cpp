#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <windows.h>
#include <conio.h>





#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define MAP_HEIGHT 30
#define MAP_WIDTH 40

using namespace std;

int speed = 0;//游戏速度，值越大，蛇运动越慢
void GotoXY(int x, int y) {//移动光标位置到x,y
	COORD pos;//定义光标位置坐标（结构体）
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//控制控制台的光标位置函数
}

typedef struct//定义坐标点结构体
{
	int x;
	int y;
}COORDINATE;


class GameSet {
public:

	static void GameInit() {//函数定义为static静态成员，不生成实体也可以直接调用

		system("mode con cols=80 lines=40");//设置游戏窗口大小

		//隐藏光标
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO CursorInfo;
		GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
		CursorInfo.bVisible = false; //隐藏控制台光标
		SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
		//初始化随机数种子
		srand((unsigned int)time(0));
	}
};

class PrintUI {
public:

	static void DrawMap() {//定义地图

		system("cls");
		int i, j;
		for (i = 0; i < MAP_WIDTH; i++) {
			cout << "*";
		}
		cout << endl;

		for (i = 0; i < MAP_HEIGHT - 2; i++) {
			cout << "*";
			for (j = 0; j < MAP_WIDTH - 2; j++) {
				cout << " ";
			}
			cout << "*" << endl;
		}

		for (i = 0; i < MAP_WIDTH; i++) {
			cout << "*";
		}
	}

	static void GameOver(int score) {//游戏结束显示得分
		GotoXY(MAP_WIDTH + 3, 3);
		cout << "游戏失败！" << endl;
		GotoXY(MAP_WIDTH + 3, 5);
		cout << "您的得分是：" << score << endl;

	}

	static void ScoreNow(int score) {//当前游戏得分
		GotoXY(MAP_WIDTH + 3, 5);
		cout << "您的得分是：" << score << endl;

		GotoXY(MAP_WIDTH + 3, 8);
		cout << "当前游戏速度: " << 10 - speed / 25 << endl;//数值越大移动速度越快

	}


};

class Food {//食物类
private:
	COORDINATE food_position;//定义食物坐标

public:
	void RandomXY(vector<COORDINATE> &snake_position) {//生成食物的随机坐标
		food_position.x = rand() % (MAP_WIDTH - 2) + 1;
		food_position.y = rand() % (MAP_HEIGHT - 2) + 1;

		unsigned int i;
		for (i = 0; i < snake_position.size(); i++) {//如果食物和蛇位置重叠，重新生成食物
			if (snake_position[i].x == food_position.x && snake_position[i].y == food_position.y) {
				food_position.x = rand() % (MAP_WIDTH - 2) + 1;
				food_position.y = rand() % (MAP_HEIGHT - 2) + 1;
				i = 0;
			}
		}
	}

	Food() {
		//默认构造函数
	}
	Food(vector<COORDINATE> &snake_position) {//重载构造函数
		RandomXY(snake_position);
	}

	void DrawFood() {//画出食物
		GotoXY(food_position.x, food_position.y);
		cout << "□";
	}

	COORDINATE GetFoodPosition() {//返回食物位置坐标
		return food_position;
	}
};

class Snake {

private:
	bool snake_alive;
	int my_direction;


public:
	vector<COORDINATE> snake_position;

	Snake() {//构造函数


		snake_alive = true;
		my_direction = 1;
		COORDINATE snake_head;
		snake_head.x = MAP_WIDTH / 2;//蛇头初始化位置
		snake_head.y = MAP_HEIGHT / 2;

		//初始蛇长3节
		this->snake_position.push_back(snake_head);//把初始的蛇头加入
		snake_head.y++;
		this->snake_position.push_back(snake_head);//在y方向向下加一节蛇身
		snake_head.y++;
		this->snake_position.push_back(snake_head);//再在y方向向下加一节蛇身


	}

	void ListenKeyboard() {//监听键盘
		char ch;

		if (_kbhit()) {//_kbhit()是非阻塞函数，检测到用户按下某键时就会返回

			ch = _getch();//getch 函数获取键盘输入
			switch (ch) {
			case 'w':
				if (this->my_direction == DOWN)
					break;
				this->my_direction = UP;
				break;
			case 's':
				if (this->my_direction == UP)
					break;
				this->my_direction = DOWN;
				break;
			case 'a':
				if (this->my_direction == RIGHT)
					break;
				this->my_direction = LEFT;
				break;
			case 'd':
				if (this->my_direction == LEFT)
					break;
				this->my_direction = RIGHT;
				break;

			case ']'://增加蛇的移动速度
				if (speed >= 25) {
					speed -= 25;
				}
				break;
			case '['://减小蛇的移动速度
				if (speed < 250) {
					speed += 25;
				}
				break;

			}
		}
	}


	void MoveSnake() {

		ListenKeyboard();
		COORDINATE head = snake_position[0];//蛇头
		//my_direction方向:1 上  2 下  3 左  4 右
		switch (this->my_direction) {//移动蛇头
		case UP:
			head.y--;
			break;
		case DOWN:
			head.y++;
			break;
		case LEFT:
			head.x--;
			break;
		case RIGHT:
			head.x++;
			break;

		}
		snake_position.insert(snake_position.begin(), head);//插入移动后的蛇头,每次移动都新加入一个蛇头，后面要减去蛇尾，保证蛇长不变
	}

	bool EatFood(Food &f) {//判断是否吃到食物
		COORDINATE food_position = f.GetFoodPosition();//获取食物坐标

		if (snake_position[0].x == food_position.x && snake_position[0].y == food_position.y) {//吃到食物，不删蛇尾

			f.RandomXY(snake_position);
			return true;
		}
		else {//没吃到食物，删除蛇尾坐标
			snake_position.erase(snake_position.end() - 1);
			return false;
		}
	}

	bool SnakeAlive() {//判断蛇死活
		if (snake_position[0].x <= 0 || snake_position[0].x >= MAP_WIDTH - 1 || snake_position[0].y <= 0 || snake_position[0].y >= MAP_HEIGHT - 1) {//蛇超出边界
			snake_alive = false;
			return snake_alive;
		}
		for (unsigned int i = 1; i < snake_position.size(); i++) {
			if (snake_position[0].x == snake_position[i].x && snake_position[0].y == snake_position[i].y) {
				//蛇碰到自己
				snake_alive = false;
				return snake_alive;
			}
		}
		snake_alive = true;
		return snake_alive;
	}

	void DrawSnake() {//画出蛇
		for (unsigned int i = 0; i < this->snake_position.size(); i++) {
			GotoXY(snake_position[i].x, snake_position[i].y);
			cout << "#";
		}
	}

	void ClearSnake() {//清除蛇尾图标
		GotoXY(snake_position[this->snake_position.size() - 1].x, snake_position[this->snake_position.size() - 1].y);
		cout << " ";
	}

	int GetSnakeSize() {//返回蛇的长度，当做得分
		return snake_position.size();
	}

};

int main() {
	GameSet setting;
	PrintUI print_ui;
	Snake snake;
	setting.GameInit();//初始化游戏

	GotoXY(MAP_WIDTH / 2 + 10, MAP_HEIGHT / 2);
	cout << "贪吃蛇" << endl;
	GotoXY(MAP_WIDTH / 2 + 10, MAP_HEIGHT / 2 + 2);
	system("pause");

	print_ui.DrawMap();
	speed = 200;
	Food food(snake.snake_position);//生成食物

	while (true) {

		print_ui.ScoreNow(snake.GetSnakeSize());//打印成绩
		food.DrawFood();//画出食物
		snake.ClearSnake();
		snake.EatFood(food);//判断是否吃到食物
		snake.MoveSnake();
		if (!snake.SnakeAlive()) {//判断蛇是否还活着
			print_ui.GameOver(snake.GetSnakeSize());
			break;
		}
		snake.DrawSnake();//画蛇

		Sleep(speed);
	}
	cin.get();

	return 0;
}