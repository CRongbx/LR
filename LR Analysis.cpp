#include "LR.h"
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>	//reverse
#include <fstream>
using namespace std;

int main(void) {
	stack<char> symbelStack;	//����ջ
	stack<int> stateStack;		//״̬ջ
	string input;				//������Ŵ�
	ofstream fout("out.txt");	//����ļ�
	bool isError = false;		//�Ƿ����

	//��ʼ��
	symbelStack.push('#');		//��������#��ʾ��
	stateStack.push(0);
	cout << "��������Ŵ���" << endl;
	cin >> input;
	input.append(1, '$');			//��������Ŵ���׷��$
	//Ϊ���������ɾ����һԪ�ز��������ַ�����ת,ÿ��ɾ�����һ��Ԫ�ؼ���erase(input.length()-1)
	reverse(input.begin(), input.end());
	
	int S, a;	//����action���ű�ĺ�������
	while (true) {
		S = stateStack.top();	//SΪ״̬ջջ��Ԫ��
		char c = input.at(input.length()-1);	//ipָ����������
		for (int i = 0; i < action_x.size();i++) {
			if (c == action_x[i]) {
				a = i;
				break;
			}
		}
		int k;		//S'
		string reduceby = "";	//��Լ���ʽ
		string getAT = Action[S][a];	//action[S,a]�ķ���ֵ

		if ('#' == getAT.at(0)) 		//��
			Error(isError, fout);
		else if ('S' == getAT.at(0)) {		//Shift S'
			k = getAT.at(1) - '0';	//S'
			if (getAT.length() > 2)
				k = 10 * k + (getAT.at(2) - '0');	//������λ�������
			symbelStack.push(c);	//�ѷ���ѹ������ջ
			stateStack.push(k);		//��S'ѹ��״̬ջ
			input.erase(input.length() - 1);	//ipָ������ƶ�һλ
		}
		else if ('R' == getAT.at(0)) {		//Reduce 
			for (int i = 2;i < getAT.length();i++)
				reduceby += getAT.at(i);
			for (int i = 1; i <= (reduceby.length() - 3);i++) {
				//�ӷ���ջ��״̬ջջ������A->B |B|���ȵķ���
				symbelStack.pop();
				stateStack.pop();
				if (reduceby.at(2+i) == 'n') 	//F->num
					i += 2;
			}
			k = stateStack.top();		//��õ�ǰ״̬ջջ��Ԫ��
			char A = reduceby.at(0);
			symbelStack.push(A);
			for (int i = 0; i < goto_x.size();i++)
				if (goto_x.at(i) == A) {
					stateStack.push(Goto[k][i]);		//ѹ������ջ��״̬ջ
					break;
				}
			fout << reduceby << endl;		//�������ʽ
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