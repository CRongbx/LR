#ifndef LR_H
#define LR_H
#include <vector>
#include <map>
#include <string>
#include <stack>
using namespace std;

string const Action[18][8] = {
	{"#","#","#","#","S4","#","S5","#"},
	{"S6","S7","#","#","#","#","#","ACC"},
	{"R:E->T","R:E->T","S8","S9","#","R:E->T","#","R:E->T"},
	{"R:T->F","R:T->F","R:T->F","R:T->F","#","R:T->F","#","R:T->F"},
	{"#","#","#","#","S4","#","S5","#"},
	{"R:F->num","R:F->num","R:F->num","R:F->num","#","R:F->num","#","R:F->num"},
	{"#","#","#","#","S4","#","S5","#"},
	{"#","#","#","#","S4","#","S5","#"},
	{"#","#","#","#","S4","#","S5","#"},
	{"#","#","#","#","S4","#","S5","#"},
	{"S6","S7","#","#","#","S15","#","#"},
	{"R:E->E+T","R:E->E+T","S16","S17","#","R:E->E+T","#","R:E->E+T"},
	{"R:E->E-T","R:E->E-T","S16","S17","#","R:E->E-T","#","R:E->E-T"},
	{"R:T->T*F","R:T->T*F","R:T->T*F","R:T->T*F","#","R:T->T*F","#","R:T->T*F"},
	{"R:T->T/F","R:T->T/F","R:T->T/F","R:T->T/F","#","R:T->T/F","#","R:T->T/F"},
	{"R:F->(E)","R:F->(E)","R:F->(E)","R:F->(E)","#","R:F->(E)","#","R:F->(E)"},
	{"#","#","#","#","S4","#","S5","#"},
	{"#","#","#","#","S4","#","S5","#"}
};

const int Goto[18][3] = {
	{1,2,3},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{10,2,3},
	{0,0,0},
	{0,11,3},
	{0,12,3},
	{0,0,13},
	{0,0,14},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,13},
	{0,0,14}
};

const vector<char> action_x = { '+','-','*','/','(',')','n','$' };
const vector<char> goto_x = { 'E','T','F' };

/* 函数声明 */
void Error(bool isError, ofstream& fout);		//简单的出错处理
void InitInput(string& input, map<size_t,int>& numMap);		//对输入符号串的初始化处理,<ip,lexval>
void ComputeValue(stack<int>& valueStack, const string production);	//根据产生式类型计算综合属性，放结果到属性栈
#endif 