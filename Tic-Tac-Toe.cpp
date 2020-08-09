/*---------------------------------------------------------------- 
// �ļ����� ������ 
// �ļ����������� �����壬Ӣ������Tic-Tac-Toe����һ����3*3�����Ͻ��е�������Ϸ����������Ƚ�����
// ������ģʽ�� 1.�˻�ģʽ  2.˫��ģʽ 3.���Զ�ս��
// ���ߣ� ���ͳ� 
// ���ʱ�䣺2016 / 5 / 2  11:00 AM 

//----------------------------------------------------------------*/
#include<iostream>
#include<cmath>
#include<cstring>
#include<iomanip>
#include<cstdlib>
#include<string>
#include<cstdio>
#include<algorithm>
#include<set>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<functional>
#include<list>
#include<deque>
#include<ctime>
#include <windows.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////
class board
{
private:
	int b[10];
	int is_oneblank(int x, int y, int c, int sign);
	bool no_blank(int x, int y, int c, int sign);
public:
	board();
	void set_board(int pos, int value);      
	int is_to_win(int sign);
	bool is_win(int sign);
	void draw();
	int operator[](int index);
};

int board::operator[](int index)    //���b��index����ֵ
{
	if (index > 0 && index < 10)
		return b[index];
	else return -10;
}
bool board::no_blank(int x, int y, int c, int sign)
{
	return b[x] == sign && b[y] == sign && b[c] == sign;
}

bool board::is_win(int sign)
{
	return (no_blank(1, 2, 3, sign) || no_blank(4, 5, 6, sign) || no_blank(7, 8, 9, sign) ||
		no_blank(1, 4, 7, sign) || no_blank(2, 5, 8, sign) || no_blank(3, 6, 9, sign) ||
		no_blank(1, 5, 9, sign) || no_blank(3, 5, 7, sign));
}

int board::is_to_win(int sign)       //Ϊ�˹����ܷ���ĺ���
{
	int pos = -99;
	pos = max(pos, is_oneblank(1, 2, 3, sign));
	pos = max(pos, is_oneblank(4, 5, 6, sign));
	pos = max(pos, is_oneblank(7, 8, 9, sign));
	pos = max(pos, is_oneblank(1, 4, 7, sign));
	pos = max(pos, is_oneblank(2, 5, 8, sign));
	pos = max(pos, is_oneblank(3, 6, 9, sign));
	pos = max(pos, is_oneblank(1, 5, 9, sign));
	pos = max(pos, is_oneblank(3, 5, 7, sign));

	return pos;
}

int board::is_oneblank(int x, int y, int c, int sign)   //Ϊ�˹����ܷ���ĺ���
{
	if (b[x] == sign && b[y] == sign && b[c] == 0)
	{
		return c;
	}
	else if (b[y] == sign && b[c] == sign && b[x] == 0)
	{
		return x;
	}
	else if (b[x] == sign && b[c] == sign && b[y] == 0)
	{
		return y;
	}
	else return -1;
}

void board::draw()     //��������
{
	for (int i = 1; i < 10; i++)
	{
		switch (b[i])
		{
		case -1:cout << "X "; break;
		case 0:cout << "_ "; break;
		case 1:cout << "O "; break;
		}

		if (i == 3 || i == 6 || i == 9)
			cout << endl << endl;
	}
}

board::board()
{
	memset(b, 0, sizeof(b));
}

void board::set_board(int pos, int value)  //����b��pos����ֵ
{
	b[pos] = value;
}



////////////////////////////////////////////////////////////////////////////

class player
{
protected:
	int signal;              //signal --- -1 for X ,0 for blank ,1 for O
	string name;
public:
	int get_signal()
	{
		return signal;
	}

	player(string name_, int signal_)
	{
		name = name_;
		signal = signal_;
	}

	void print_turn()
	{
		cout << "���ڸ�" << name << "������" << endl;
	}

	string get_name()
	{
		return name;
	}

	virtual int get_pos(board b) = 0;
};


////////////////////////////////////////////////////////////////////////////

class playerHuman : public player
{
public:
	playerHuman(string _name, int sign) :player(_name, sign) {}
	int get_pos(board b);
};

int playerHuman::get_pos(board b)    //�Ӽ��̻��λ��
{
	int user_pos;
	cout << "�����ӣ��������� 1~9����" << endl;

	while (cin >> user_pos, b[user_pos] != 0)
	{
		cout << "��λ���Ѿ������ӣ��� ����������" << endl;
	}
	cout << endl;
	return user_pos;
}


////////////////////////////////////////////////////////////////////////////

class playerComputer : public player
{
private:
	int value_of_pos[10];
public:
	playerComputer(string _name, int sign) :player(_name, sign) {}
	pair<int, int> get_max_pos_value(int step, const board& bo);//�˹����ܹؼ�����
	int get_pos(board b);
};

int playerComputer::get_pos(board b)  //�����˹����ܻ��λ��
{
	int pos = get_max_pos_value(1, b).first;
	cout << "�������� " << pos << " ��λ��" << endl << endl;
	return pos;
}


/*----------------------�˹����ܹؼ������㷨˵��----------------------------

step��ʾ�ӵ�ǰ��ʼ����AIΪ��һ������Ϊֻ��AI�Ż���ô˺����������� �ڶ��� ������ ���� 	
����ʱ�����⵽��ǰ�ǵ������ӣ����ҳ������Ա�ʤ���棨�磺1λ��Ϊ���Ե����ӣ�3λ��Ϊ���Ե����ӣ���2λ��Ϊ�գ� 
��ô��ֱ�ӷ���pair��2���յ��Ǹ�λ�ã���1����
ͬ�����統ǰ�������������Ա�ʤ ��ô��ֱ�ӷ���pair���յ��Ǹ�λ�ã�-1����
����������˻�û���Ա�ʤ���棬�򷵻� 0 ����ʾƽ�֣���
˫������ȡ���Ų��ԣ�Ҳ����˵���Խ�ѡ�����п��ܵ����ӵ�����Ȩ�����ģ�
���˽�ѡ�����п��ܵ����ӵ�����Ȩ����С�ġ�
�������ν����С�㷨�� 
------------------------------------------------------------------------------*/
pair<int, int> playerComputer::get_max_pos_value(int step, const board& bo)//�˹����ܹؼ�����
{																 
	board b = bo;   
	int max_ = -999;  
	int min_ = 999; 
	int value = 0, pos;
	if (step % 2 == 1)  
	{
		int final_pos = b.is_to_win(signal);
		if (final_pos != -1) 
		{
			return make_pair(final_pos, 1);
		}
		for (int i = 1; i <= 9; i++)
		{
			if (b[i] == 0)
			{

				b.set_board(i, signal);
				value_of_pos[i] = get_max_pos_value(step + 1, b).second;
				if (value_of_pos[i] > max_)
				{
					value = value_of_pos[i];
					pos = i;
					max_ = value_of_pos[i];
				}
				b.set_board(i, 0);
			}
		}
	}
	else
	{

		int op_signal = (signal == -1) ? 1 : -1;
		int final_pos = b.is_to_win(op_signal);
		if (final_pos != -1)
		{
			return make_pair(final_pos, -1);
		}
		for (int i = 1; i <= 9; i++)
		{
			if (b[i] == 0)
			{
				b.set_board(i, op_signal);
				value_of_pos[i] = get_max_pos_value(step + 1, b).second;
				if (value_of_pos[i] < min_)
				{
					value = value_of_pos[i];
					pos = i;
					min_ = value_of_pos[i];
				}
				b.set_board(i, 0);
			}
		}
	}
	return make_pair(pos, value);
}                                                 //�˹����ܹؼ��������� 


////////////////////////////////////////////////////////////////////////////

class game
{
private:
	board bo;
	player* players[2];      
	int game_state;   //0��ʾ���ֻ�û������1��ʾ����
public:
	game(int mode_);
	void print_set_game_state();
	int get_game_state();
	int get_player_pos(int index);
	void draw_board();
	void print_message(int pos);
	void set_board(int pos, int value);

};

void game::print_set_game_state()    //�������״̬������Ӯ����� XXXӮ�ˣ�û��Ӯ�����
{
	if (bo.is_win(players[0]->get_signal()))
	{
		cout << "������������" << players[0]->get_name() << "Ӯ�ˣ���������������" << endl << endl;
		game_state = 1;
	}
	else if (bo.is_win(players[1]->get_signal()))
	{
		cout << "������������" << players[1]->get_name() << "Ӯ�ˣ���������������" << endl << endl;
		game_state = 1;
	}
}

int game::get_game_state()
{
	return game_state;
}

int game::get_player_pos(int index)       //����player��get_pos������������һ��Ҫ�ߵ�λ��
{
	return players[index]->get_pos(bo);
}

game::game(int mode_)     //����ģʽ����Ӧ�Ķ������players[]�������棬���˻�ģʽ����һ��playerHuman�����һ��playerComputer����
{                         //mode_(��Ϸģʽ)  1.�˻�ģʽ  2.˫��ģʽ 3.���Զ�ս
	game_state = 0;
	if (mode_ == 1)
	{
		cout << "���Ƿ�Ҫ���ߡ��� 1.����  2.����  ������ \"1\" ���� \"2\"" << endl;
		int is_first;
		cin >> is_first;
		playerHuman* pHuman = new playerHuman("Human", (is_first == 1) ? -1 : 1);
		players[(is_first == 1) ? 0 : 1] = pHuman;
		playerComputer* pComputer = new playerComputer("Computer", (is_first == 1) ? 1 : -1);
		players[(is_first == 1) ? 1 : 0] = pComputer;
	}
	else if (mode_ == 2)
	{
		cout << "�Ƿ� ���A Ҫ���ߡ��� 1.����  2.����  ������ \"1\" ���� \"2\"" << endl;
		int is_first;
		cin >> is_first;
		playerHuman* pHumanA = new playerHuman("HumanA", (is_first == 1) ? -1 : 1);
		players[(is_first == 1) ? 0 : 1] = pHumanA;
		playerHuman* pHumanB = new playerHuman("HumanB", (is_first == 1) ? 1 : -1);
		players[(is_first == 1) ? 1 : 0] = pHumanB;
	}
	else
	{
		cout << "�Ƿ� ����A Ҫ���ߡ��� 1.����  2.����  ������ \"1\" ���� \"2\"" << endl;
		int is_first;
		cin >> is_first;
		playerComputer* pComputerA = new playerComputer("ComputerA", (is_first == 1) ? -1 : 1);
		players[(is_first == 1) ? 0 : 1] = pComputerA;
		playerComputer* pComputerB = new playerComputer("ComputerB", (is_first == 1) ? 1 : -1);
		players[(is_first == 1) ? 1 : 0] = pComputerB;

	}
}


void game::set_board(int pos, int value)
{
	bo.set_board(pos, value);
}


void game::draw_board()
{
	bo.draw();
}

void game::print_message(int pos)
{
	players[pos]->print_turn();
}

////////////////////////////////////////////////////////////////////////////


int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);      //���������ʽ����
	cout << "��ӭ���� ������ ��Ϸ!" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
                       //���ú�ɫ
	cout << "����Ϸ�� 1~9 �������꣨���� ��2�е�3�� Ϊ 6���� ������Ϊ'X',����Ϊ'O'" << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "��Ϸ��ʼ�� ��ѡ����Ϸģʽ���� 1.�˻�ģʽ  2.˫��ģʽ 3.���Զ�ս��\n������ \"1\" ���� \"2\" ���� \"3\"   ����\"0\"�˳���Ϸ" << endl;
	int user_mode;
	while (cin >> user_mode, user_mode)
	{
		game OXgame(user_mode);
		bool flag = false;

		for (int i = 1; i <= 9; i++)
		{
			if (i % 2 == 1)
			{
				OXgame.print_message(0);
				
				OXgame.set_board(OXgame.get_player_pos(0), -1);

			}
			else
			{
				OXgame.print_message(1);

				OXgame.set_board(OXgame.get_player_pos(1), 1);
			}

			OXgame.draw_board();

			OXgame.print_set_game_state();

			if (OXgame.get_game_state() == 1)
			{
				break;
			}
		}

		if (OXgame.get_game_state() != 1)
		{
			cout << "������������ƽ�֣���������������" << endl << endl;
		}
		cout << "��Ϸ��ʼ�� ��ѡ����Ϸģʽ���� 1.�˻�ģʽ  2.˫��ģʽ 3.���Զ�ս��\n������ \"1\" ���� \"2\" ���� \"3\"   ����\"0\"�˳���Ϸ" << endl;

	}
}
