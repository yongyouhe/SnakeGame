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

int speed = 0;//��Ϸ�ٶȣ�ֵԽ�����˶�Խ��
void GotoXY(int x, int y) {//�ƶ����λ�õ�x,y
	COORD pos;//������λ�����꣨�ṹ�壩
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//���ƿ���̨�Ĺ��λ�ú���
}

typedef struct//���������ṹ��
{
	int x;
	int y;
}COORDINATE;


class GameSet {
public:

	static void GameInit() {//��������Ϊstatic��̬��Ա��������ʵ��Ҳ����ֱ�ӵ���

		system("mode con cols=80 lines=40");//������Ϸ���ڴ�С

		//���ع��
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO CursorInfo;
		GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
		CursorInfo.bVisible = false; //���ؿ���̨���
		SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
		//��ʼ�����������
		srand((unsigned int)time(0));
	}
};

class PrintUI {
public:

	static void DrawMap() {//�����ͼ

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

	static void GameOver(int score) {//��Ϸ������ʾ�÷�
		GotoXY(MAP_WIDTH + 3, 3);
		cout << "��Ϸʧ�ܣ�" << endl;
		GotoXY(MAP_WIDTH + 3, 5);
		cout << "���ĵ÷��ǣ�" << score << endl;

	}

	static void ScoreNow(int score) {//��ǰ��Ϸ�÷�
		GotoXY(MAP_WIDTH + 3, 5);
		cout << "���ĵ÷��ǣ�" << score << endl;

		GotoXY(MAP_WIDTH + 3, 8);
		cout << "��ǰ��Ϸ�ٶ�: " << 10 - speed / 25 << endl;//��ֵԽ���ƶ��ٶ�Խ��

	}


};

class Food {//ʳ����
private:
	COORDINATE food_position;//����ʳ������

public:
	void RandomXY(vector<COORDINATE> &snake_position) {//����ʳ����������
		food_position.x = rand() % (MAP_WIDTH - 2) + 1;
		food_position.y = rand() % (MAP_HEIGHT - 2) + 1;

		unsigned int i;
		for (i = 0; i < snake_position.size(); i++) {//���ʳ�����λ���ص�����������ʳ��
			if (snake_position[i].x == food_position.x && snake_position[i].y == food_position.y) {
				food_position.x = rand() % (MAP_WIDTH - 2) + 1;
				food_position.y = rand() % (MAP_HEIGHT - 2) + 1;
				i = 0;
			}
		}
	}

	Food() {
		//Ĭ�Ϲ��캯��
	}
	Food(vector<COORDINATE> &snake_position) {//���ع��캯��
		RandomXY(snake_position);
	}

	void DrawFood() {//����ʳ��
		GotoXY(food_position.x, food_position.y);
		cout << "��";
	}

	COORDINATE GetFoodPosition() {//����ʳ��λ������
		return food_position;
	}
};

class Snake {

private:
	bool snake_alive;
	int my_direction;


public:
	vector<COORDINATE> snake_position;

	Snake() {//���캯��


		snake_alive = true;
		my_direction = 1;
		COORDINATE snake_head;
		snake_head.x = MAP_WIDTH / 2;//��ͷ��ʼ��λ��
		snake_head.y = MAP_HEIGHT / 2;

		//��ʼ�߳�3��
		this->snake_position.push_back(snake_head);//�ѳ�ʼ����ͷ����
		snake_head.y++;
		this->snake_position.push_back(snake_head);//��y�������¼�һ������
		snake_head.y++;
		this->snake_position.push_back(snake_head);//����y�������¼�һ������


	}

	void ListenKeyboard() {//��������
		char ch;

		if (_kbhit()) {//_kbhit()�Ƿ�������������⵽�û�����ĳ��ʱ�ͻ᷵��

			ch = _getch();//getch ������ȡ��������
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

			case ']'://�����ߵ��ƶ��ٶ�
				if (speed >= 25) {
					speed -= 25;
				}
				break;
			case '['://��С�ߵ��ƶ��ٶ�
				if (speed < 250) {
					speed += 25;
				}
				break;

			}
		}
	}


	void MoveSnake() {

		ListenKeyboard();
		COORDINATE head = snake_position[0];//��ͷ
		//my_direction����:1 ��  2 ��  3 ��  4 ��
		switch (this->my_direction) {//�ƶ���ͷ
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
		snake_position.insert(snake_position.begin(), head);//�����ƶ������ͷ,ÿ���ƶ����¼���һ����ͷ������Ҫ��ȥ��β����֤�߳�����
	}

	bool EatFood(Food &f) {//�ж��Ƿ�Ե�ʳ��
		COORDINATE food_position = f.GetFoodPosition();//��ȡʳ������

		if (snake_position[0].x == food_position.x && snake_position[0].y == food_position.y) {//�Ե�ʳ���ɾ��β

			f.RandomXY(snake_position);
			return true;
		}
		else {//û�Ե�ʳ�ɾ����β����
			snake_position.erase(snake_position.end() - 1);
			return false;
		}
	}

	bool SnakeAlive() {//�ж�������
		if (snake_position[0].x <= 0 || snake_position[0].x >= MAP_WIDTH - 1 || snake_position[0].y <= 0 || snake_position[0].y >= MAP_HEIGHT - 1) {//�߳����߽�
			snake_alive = false;
			return snake_alive;
		}
		for (unsigned int i = 1; i < snake_position.size(); i++) {
			if (snake_position[0].x == snake_position[i].x && snake_position[0].y == snake_position[i].y) {
				//�������Լ�
				snake_alive = false;
				return snake_alive;
			}
		}
		snake_alive = true;
		return snake_alive;
	}

	void DrawSnake() {//������
		for (unsigned int i = 0; i < this->snake_position.size(); i++) {
			GotoXY(snake_position[i].x, snake_position[i].y);
			cout << "#";
		}
	}

	void ClearSnake() {//�����βͼ��
		GotoXY(snake_position[this->snake_position.size() - 1].x, snake_position[this->snake_position.size() - 1].y);
		cout << " ";
	}

	int GetSnakeSize() {//�����ߵĳ��ȣ������÷�
		return snake_position.size();
	}

};

int main() {
	GameSet setting;
	PrintUI print_ui;
	Snake snake;
	setting.GameInit();//��ʼ����Ϸ

	GotoXY(MAP_WIDTH / 2 + 10, MAP_HEIGHT / 2);
	cout << "̰����" << endl;
	GotoXY(MAP_WIDTH / 2 + 10, MAP_HEIGHT / 2 + 2);
	system("pause");

	print_ui.DrawMap();
	speed = 200;
	Food food(snake.snake_position);//����ʳ��

	while (true) {

		print_ui.ScoreNow(snake.GetSnakeSize());//��ӡ�ɼ�
		food.DrawFood();//����ʳ��
		snake.ClearSnake();
		snake.EatFood(food);//�ж��Ƿ�Ե�ʳ��
		snake.MoveSnake();
		if (!snake.SnakeAlive()) {//�ж����Ƿ񻹻���
			print_ui.GameOver(snake.GetSnakeSize());
			break;
		}
		snake.DrawSnake();//����

		Sleep(speed);
	}
	cin.get();

	return 0;
}