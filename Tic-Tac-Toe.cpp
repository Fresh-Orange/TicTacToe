/*---------------------------------------------------------------- 
// 文件名： 井字棋 
// 文件功能描述： 井字棋，英文名叫Tic-Tac-Toe，是一种在3*3格子上进行的连珠游戏，和五子棋比较类似
// 有三种模式： 1.人机模式  2.双人模式 3.电脑对战；
// 作者： 赖贤城 
// 完成时间：2016 / 5 / 2  11:00 AM 

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

int board::operator[](int index)    //获得b【index】的值
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

int board::is_to_win(int sign)       //为人工智能服务的函数
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

int board::is_oneblank(int x, int y, int c, int sign)   //为人工智能服务的函数
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

void board::draw()     //画出棋盘
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

void board::set_board(int pos, int value)  //设置b【pos】的值
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
		cout << "现在该" << name << "下子了" << endl;
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

int playerHuman::get_pos(board b)    //从键盘获得位置
{
	int user_pos;
	cout << "请落子（输入坐标 1~9）：" << endl;

	while (cin >> user_pos, b[user_pos] != 0)
	{
		cout << "该位置已经有棋子！！ 请重新输入" << endl;
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
	pair<int, int> get_max_pos_value(int step, const board& bo);//人工智能关键函数
	int get_pos(board b);
};

int playerComputer::get_pos(board b)  //调用人工智能获得位置
{
	int pos = get_max_pos_value(1, b).first;
	cout << "它下在了 " << pos << " 的位置" << endl << endl;
	return pos;
}


/*----------------------人工智能关键函数算法说明----------------------------

step表示从当前开始，以AI为第一步（因为只有AI才会调用此函数）紧接着 第二步 是人下 …… 	
搜索时如果检测到当前是电脑下子，而且出现明显必胜局面（如：1位置为电脑的棋子，3位置为电脑的棋子，而2位置为空） 
那么就直接返回pair（2（空的那个位置），1）；
同理，假如当前是人下子且明显必胜 那么就直接返回pair（空的那个位置，-1）；
如果棋盘满了还没明显必胜局面，则返回 0 （表示平局）。
双方都采取最优策略，也就是说电脑将选择所有可能的下子点里面权重最大的，
而人将选择所有可能的下子点里面权重最小的。
这就是所谓极大极小算法。 
------------------------------------------------------------------------------*/
pair<int, int> playerComputer::get_max_pos_value(int step, const board& bo)//人工智能关键函数
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
}                                                 //人工智能关键函数结束 


////////////////////////////////////////////////////////////////////////////

class game
{
private:
	board bo;
	player* players[2];      
	int game_state;   //0表示本局还没结束，1表示结束
public:
	game(int mode_);
	void print_set_game_state();
	int get_game_state();
	int get_player_pos(int index);
	void draw_board();
	void print_message(int pos);
	void set_board(int pos, int value);

};

void game::print_set_game_state()    //输出比赛状态，有人赢则输出 XXX赢了，没人赢则不输出
{
	if (bo.is_win(players[0]->get_signal()))
	{
		cout << "——————" << players[0]->get_name() << "赢了！！——————" << endl << endl;
		game_state = 1;
	}
	else if (bo.is_win(players[1]->get_signal()))
	{
		cout << "——————" << players[1]->get_name() << "赢了！！——————" << endl << endl;
		game_state = 1;
	}
}

int game::get_game_state()
{
	return game_state;
}

int game::get_player_pos(int index)       //调用player的get_pos函数获得玩家下一步要走的位置
{
	return players[index]->get_pos(bo);
}

game::game(int mode_)     //根据模式将相应的对象加入players[]数组里面，如人机模式加入一个playerHuman对象和一个playerComputer对象
{                         //mode_(游戏模式)  1.人机模式  2.双人模式 3.电脑对战
	game_state = 0;
	if (mode_ == 1)
	{
		cout << "您是否要先走—— 1.先走  2.后走  请输入 \"1\" 或者 \"2\"" << endl;
		int is_first;
		cin >> is_first;
		playerHuman* pHuman = new playerHuman("Human", (is_first == 1) ? -1 : 1);
		players[(is_first == 1) ? 0 : 1] = pHuman;
		playerComputer* pComputer = new playerComputer("Computer", (is_first == 1) ? 1 : -1);
		players[(is_first == 1) ? 1 : 0] = pComputer;
	}
	else if (mode_ == 2)
	{
		cout << "是否 玩家A 要先走—— 1.先走  2.后走  请输入 \"1\" 或者 \"2\"" << endl;
		int is_first;
		cin >> is_first;
		playerHuman* pHumanA = new playerHuman("HumanA", (is_first == 1) ? -1 : 1);
		players[(is_first == 1) ? 0 : 1] = pHumanA;
		playerHuman* pHumanB = new playerHuman("HumanB", (is_first == 1) ? 1 : -1);
		players[(is_first == 1) ? 1 : 0] = pHumanB;
	}
	else
	{
		cout << "是否 电脑A 要先走—— 1.先走  2.后走  请输入 \"1\" 或者 \"2\"" << endl;
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
		FOREGROUND_GREEN | FOREGROUND_BLUE);      //设置输出格式而已
	cout << "欢迎来到 井字棋 游戏!" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
                       //设置红色
	cout << "该游戏以 1~9 代表坐标（例如 第2行第3列 为 6）， 且先手为'X',后手为'O'" << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "游戏开始： 请选择游戏模式—— 1.人机模式  2.双人模式 3.电脑对战；\n请输入 \"1\" 或者 \"2\" 或者 \"3\"   或者\"0\"退出游戏" << endl;
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
			cout << "——————平局！！——————" << endl << endl;
		}
		cout << "游戏开始： 请选择游戏模式—— 1.人机模式  2.双人模式 3.电脑对战；\n请输入 \"1\" 或者 \"2\" 或者 \"3\"   或者\"0\"退出游戏" << endl;

	}
}
