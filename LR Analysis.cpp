#include "LR.h"
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>	//reverse
#include <fstream>
using namespace std;
#define EMPTYNUM -6666		//综合属性中的空

int main(void) {
	stack<char> symbelStack;	//符号栈
	stack<int> stateStack;		//状态栈
	stack<int> valueStack;		//属性栈
	string input;				//输入符号串
	ofstream fout("out.txt");	//输出文件
	bool isError = false;		//是否出错
	map<size_t, int>numMap;

	//初始化
	symbelStack.push('#');		//本程序用#表示空
	valueStack.push(EMPTYNUM);
	stateStack.push(0);
	cout << "请输入符号串：" << endl;
	cin >> input;
	InitInput(input, numMap);
	
	int S, a;	//对于action符号表的横纵坐标
	int ip = 0;
	while (true) {
		S = stateStack.top();	//S为状态栈栈顶元素
		char c = input.at(ip);	//ip指向的输入符号
		for (int i = 0; i < action_x.size();i++) {
			if (c == action_x[i]) {
				a = i;
				break;
			}
		}
		int k = 0;		//S'
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
			if (c == 'n') {
				//num
				int num = numMap[ip];	//c对应的num的int值
				valueStack.push(num);
			}
			else
				valueStack.push(EMPTYNUM);
			ip++;	//ip指针向后移动一位
		}
		else if ('R' == getAT.at(0)) {		//Reduce 
			//获得规约产生式
			for (int i = 2;i < getAT.length();i++)
				reduceby += getAT.at(i);
			//在规约前先计算val的值
			ComputeValue(valueStack, reduceby);
			//规约产生式，弹出符号栈和状态栈
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
			fout << reduceby << ";\t";		//输出产生式
		}
		else if ('A' == getAT.at(0)) {		//Accept
			if (!isError) {
				fout << endl << "SUCCESS!" << endl;
				fout << "RESULT: " << valueStack.top() << endl;
			}
			break;
		}
	}

	system("pause");
	return 0;
}

void Error(bool isError, ofstream& fout) {
	isError = true;
	fout << endl << "ERROR!" << endl;
	exit(-1);	//出错直接退出程序
}

/*对输入符号串的初始化处理，主要需要完成功能如下：
* 1、添加$
* 2、对num.lexval赋值*/
void InitInput(string& input, map<size_t, int>& numMap) {
	//1. 向输入符号串后追加$
	input.append(1, '$');			
	//2. 对num.lexval赋值
	//用Ip操作input，将数字转化为字符n，同时将Ip与lexval记录到一张map<ip,lexval>里，翻译时直接查表
	int ip = 0;
	char c = '\0';
	int lexval = 0;	//记录id.lexval
	numMap.clear();	//初始化清空map
	size_t len = 0;	//连续数字位数
	for (size_t i = 0; i < input.length(); i++) {
		if (isdigit(input.at(i))) {
			lexval = lexval * 10 + input.at(i) - '0';
			len++;
		}
		else {
			if (len != 0) {
				//前面处理的是数字
				numMap.insert(pair<size_t, int>(i - len, lexval));
				//输入字符串用n替换数字,其余位数删除
				input.erase(i - len, len);
				input.insert(i - len, 1, 'n');
				i = i - len + 1;
				len = 0; lexval = 0;
			}
		}
	}
}

void ComputeValue(stack<int>& valueStack, const string production) {
	//根据产生式类型计算综合属性，放结果到属性栈
	if (production == "E->E+T") {
		int a, b;
		a = valueStack.top();	//E1.val
		valueStack.pop();		//E1
		valueStack.pop();		// /
		b = valueStack.top();	//T.val
		valueStack.pop();		//T.val
		valueStack.push(a + b);
	}
	else if (production == "E->E-T") {
		int a, b;
		a = valueStack.top();	//E1.val
		valueStack.pop();		//E1
		valueStack.pop();		// /
		b = valueStack.top();	//T.val
		valueStack.pop();		//T.val
		valueStack.push(b - a);
	}
	else if (production == "E->T") {
		;	//valStack not change
	}
	else if (production == "T->T*F") {
		int a, b;
		a = valueStack.top();	//T1.val
		valueStack.pop();		//T
		valueStack.pop();		// *
		b = valueStack.top();	//F.val
		valueStack.pop();		//F.val
		valueStack.push(a * b);
	}
	else if (production == "T->T/F") {
		int a, b;
		a = valueStack.top();	//T1.val
		valueStack.pop();		//T
		valueStack.pop();		// /
		b = valueStack.top();	//F.val
		valueStack.pop();		//F.val
		valueStack.push(b/a);
	}
	else if (production == "T->F") {
		;	//valStack not change
	}
	else if (production == "F->(E)") {
		valueStack.pop();	//(
		int temp = valueStack.top();
		valueStack.pop();	//E.val
		valueStack.pop();	//)
		valueStack.push(temp);	
	}
	else if (production == "F->num") {
		;	//valStack not change
	}
}