#include "LR.h"
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>	//reverse
#include <fstream>
using namespace std;

int main(void) {
	stack<char> symbelStack;	//符号栈
	stack<int> stateStack;		//状态栈
	string input;				//输入符号串
	ofstream fout("out.txt");	//输出文件
	bool isError = false;		//是否出错

	//初始化
	symbelStack.push('#');		//本程序用#表示空
	stateStack.push(0);
	cout << "请输入符号串：" << endl;
	cin >> input;
	input.append(1, '$');			//向输入符号串后追加$
	//为方便后续串删除第一元素操作，将字符串逆转,每次删除最后一个元素即可erase(input.length()-1)
	reverse(input.begin(), input.end());
	
	int S, a;	//对于action符号表的横纵坐标
	while (true) {
		S = stateStack.top();	//S为状态栈栈顶元素
		char c = input.at(input.length()-1);	//ip指向的输入符号
		for (int i = 0; i < action_x.size();i++) {
			if (c == action_x[i]) {
				a = i;
				break;
			}
		}
		int k;		//S'
		string reduceby = "";	//规约表达式
		string getAT = Action[S][a];	//action[S,a]的返回值

		if ('#' == getAT.at(0)) 		//空
			Error(isError, fout);
		else if ('S' == getAT.at(0)) {		//Shift S'
			k = getAT.at(1) - '0';	//S'
			if (getAT.length() > 2)
				k = 10 * k + (getAT.at(2) - '0');	//处理两位数的情况
			symbelStack.push(c);	//把符号压进符号栈
			stateStack.push(k);		//把S'压到状态栈
			input.erase(input.length() - 1);	//ip指针向后移动一位
		}
		else if ('R' == getAT.at(0)) {		//Reduce 
			for (int i = 2;i < getAT.length();i++)
				reduceby += getAT.at(i);
			for (int i = 1; i <= (reduceby.length() - 3);i++) {
				//从符号栈和状态栈栈顶弹出A->B |B|长度的符号
				symbelStack.pop();
				stateStack.pop();
				if (reduceby.at(2+i) == 'n') 	//F->num
					i += 2;
			}
			k = stateStack.top();		//获得当前状态栈栈顶元素
			char A = reduceby.at(0);
			symbelStack.push(A);
			for (int i = 0; i < goto_x.size();i++)
				if (goto_x.at(i) == A) {
					stateStack.push(Goto[k][i]);		//压进符号栈和状态栈
					break;
				}
			fout << reduceby << endl;		//输出产生式
		}
		else if ('A' == getAT.at(0)) {		//Accept
			if (!isError)
				fout << "SUCCESS!" << endl;
			break;
		}
	}

	system("pause");
	return 0;
}

void Error(bool isError, ofstream& fout) {
	isError = true;
	fout << "ERROR!" << endl;
}