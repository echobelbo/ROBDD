#include"ROBDD.h"
#include<stdlib.h>
#include<iostream>
#include<string>
#include<stack>


int pow(int pow)
{
	int result = 1;
	for (int i = 0; i < pow; i++)
	{
		result *= 10;
	}
	return result;
}//求10的方幂

BDDNode* initBDDNode(int mode, int value, BDDNode* father)
{
	BDDNode* newNode = (BDDNode*)malloc(sizeof(BDDNode));

	newNode->type = mode;
	newNode->value = value;
	newNode->fatherptr = father;
	newNode->leftptr = NULL;
	newNode->rightptr = NULL;
	if (newNode->type == 2)
	{
		newNode->leftptr = initBDDNode(0, 0, newNode);
		newNode->rightptr = initBDDNode(0, 1, newNode);
	}
	return newNode;
}

void printBDDNode(BDDNode* root, string before)
{
	before += ' ';
	if (root->type == 0)
	{
		before += root->value ? "True" : "False";
		cout << before << endl;
		return;
	}
	if (root->type == 1)
	{
		char op[3];
		op[2] = '\0';
		switch (root->value)
		{
			case 0: op[0] = '/'; op[1] = '\\'; break;
			case 1: op[0] = '\\'; op[1] = '/'; break;
			case 2: op[0] = '='; op[1] = '>'; break;
			case 3: op[0] = '='; op[1] = '='; break;
			default: exit(-1);
			break;
		}
		before += op;
		before += ' ';
		printBDDNode(root->leftptr, before + '-');
		printBDDNode(root->rightptr, before + '+');
		return;
	}
	if (root->type == 2)
	{
		char p[64];
		snprintf(p, 64, "%d", root->value);
		before += p;
		before += ' ';
		printBDDNode(root->leftptr, before + '-');
		printBDDNode(root->rightptr, before + '+');
		return;
	}
	exit(-1);
}

BDDNode* GenerateBDDtree(string input)
{
	stack<char> char_stack;
	stack<BDDNode*> BDD_stack;
	char_stack.push(input[0]);
	int len = input.length();
	int char_stack_top = 0;
	int flag = 1;
	while (flag < len)
	{
		
		if (input[flag] != ')')
		{
			char_stack.push(input[flag]);
			char_stack_top++;
		}
		else
		{
			int op1_pow = 0;
			int op2_pow = 0;
			int op1 = 0;
			int op2 = 0;
			bool op1_mode = false;
			bool op2_mode = false;
			bool op2_ready = false;
			//op1为op1的操作数值，op1_pow是用来计算op1的10方幂次数，op1_mode为false时说明要使用命题节点，true时使用运算符节点
			int mode = 4;
			char temp = char_stack.top();
			while (temp != '(')
			{
				
				switch (temp)
				{
				case('&'): mode = 0; op2_ready = true; break;
				case('|'): mode = 1; op2_ready = true; break;
				case('>'): mode = 2; op2_ready = true; break;
				case('='): mode = 3; op2_ready = true; break;
				default:
					if (temp == '*')
					{
						if (op2_ready)
							op1_mode = true;
						else
							op2_mode = true;
						break;
					}
					else 
					{
						if (op2_ready)
							op1 += (temp - '0' + 0) * pow(op1_pow++);
						else
							op2 += (temp - '0' + 0) * pow(op2_pow++);
						break;
					}	
				}
				char_stack.pop();
				char_stack_top--;
				temp = char_stack.top();
			}
			char_stack.pop();
			BDDNode* opNode = initBDDNode(1, mode, NULL);
			BDDNode* op1Node;
			BDDNode* op2Node;
			if (op2_mode)
			{
				op2Node = BDD_stack.top();
				BDD_stack.pop();
			}
			else
			{
				op2Node = initBDDNode(2, op2, opNode);
			}
			if (op1_mode)
			{
				op1Node = BDD_stack.top();
				BDD_stack.pop();
			}
			else
			{
				op1Node = initBDDNode(2, op1, opNode);
			}
			opNode->leftptr = op1Node;
			opNode->rightptr = op2Node;
			BDD_stack.push(opNode);
			char_stack.push('*');
		}
		flag++;
	}
	BDDNode* root = BDD_stack.top();
	return root;
}