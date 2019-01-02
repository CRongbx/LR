#include "LR.h"
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>	//reverse
#include <fstream>
using namespace std;
#define EMPTYNUM -6666		//�ۺ������еĿ�

int main(void) {
	stack<char> symbelStack;	//����ջ
	stack<int> stateStack;		//״̬ջ
	stack<int> valueStack;		//����ջ
	string input;				//������Ŵ�
	ofstream fout("out.txt");	//����ļ�
	bool isError = false;		//�Ƿ����
	map<size_t, int>numMap;

	//��ʼ��
	symbelStack.push('#');		//��������#��ʾ��
	valueStack.push(EMPTYNUM);
	stateStack.push(0);
	cout << "��������Ŵ���" << endl;
	cin >> input;
	InitInput(input, numMap);
	
	int S, a;	//����action���ű�ĺ�������
	int ip = 0;
	while (true) {
		S = stateStack.top();	//SΪ״̬ջջ��Ԫ��
		char c = input.at(ip);	//ipָ����������
		for (int i = 0; i < action_x.size();i++) {
			if (c == action_x[i]) {
				a = i;
				break;
			}
		}
		int k = 0;		//S'
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
			if (c == 'n') {
				//num
				int num = numMap[ip];	//c��Ӧ��num��intֵ
				valueStack.push(num);
			}
			else
				valueStack.push(EMPTYNUM);
			ip++;	//ipָ������ƶ�һλ
		}
		else if ('R' == getAT.at(0)) {		//Reduce 
			//��ù�Լ����ʽ
			for (int i = 2;i < getAT.length();i++)
				reduceby += getAT.at(i);
			//�ڹ�Լǰ�ȼ���val��ֵ
			ComputeValue(valueStack, reduceby);
			//��Լ����ʽ����������ջ��״̬ջ
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
			fout << reduceby << ";\t";		//�������ʽ
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
	exit(-1);	//����ֱ���˳�����
}

/*��������Ŵ��ĳ�ʼ��������Ҫ��Ҫ��ɹ������£�
* 1�����$
* 2����num.lexval��ֵ*/
void InitInput(string& input, map<size_t, int>& numMap) {
	//1. ��������Ŵ���׷��$
	input.append(1, '$');			
	//2. ��num.lexval��ֵ
	//��Ip����input��������ת��Ϊ�ַ�n��ͬʱ��Ip��lexval��¼��һ��map<ip,lexval>�����ʱֱ�Ӳ��
	int ip = 0;
	char c = '\0';
	int lexval = 0;	//��¼id.lexval
	numMap.clear();	//��ʼ�����map
	size_t len = 0;	//��������λ��
	for (size_t i = 0; i < input.length(); i++) {
		if (isdigit(input.at(i))) {
			lexval = lexval * 10 + input.at(i) - '0';
			len++;
		}
		else {
			if (len != 0) {
				//ǰ�洦���������
				numMap.insert(pair<size_t, int>(i - len, lexval));
				//�����ַ�����n�滻����,����λ��ɾ��
				input.erase(i - len, len);
				input.insert(i - len, 1, 'n');
				i = i - len + 1;
				len = 0; lexval = 0;
			}
		}
	}
}

void ComputeValue(stack<int>& valueStack, const string production) {
	//���ݲ���ʽ���ͼ����ۺ����ԣ��Ž��������ջ
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