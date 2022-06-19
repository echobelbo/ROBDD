#include"ROBDD.h"
#include<stdlib.h>
#include<iostream>
#include<string>
#include<stack>
#include<fstream>

ofstream out_path;
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
	newNode->name = -1;
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
BDDNode* ROBDDmain(BDDNode* root)
{
	BDDNode* left = root->leftptr;
	BDDNode* right = root->rightptr;
	if (left->type == 1)
	{
		left = ROBDDmain(left);
		root->leftptr = left;
	}
	if (right->type == 1)
	{
		right = ROBDDmain(right);
		root->rightptr = right;
	}
	if (left->type == 0 && right->type == 0)
	{
		switch (root->value)
		{
		case(0):root->value = right->value & left->value; break;
		case(1):root->value = right->value | left->value; break;
		case(2):root->value = left->value ? (right->value ? 1 : 0) : 1; break;
		case(3):root->value = (left->value == right->value) ? 1 : 0; break;
		default:
			break;
		}
		root->type = 0;
		//free(left);
		//free(right);
		return root;
	}//如果运算符节点左右都为叶子节点，进行运算
	if (left->type == 2 && right->type == 2 && left->value == right->value)
	{
		BDDNode* new_left, * new_right, * new_root;
		new_root = initBDDNode(2, left->value, root->fatherptr);
		new_left = initBDDNode(1, root->value, new_root);
		new_right = initBDDNode(1, root->value, new_root);

		new_left->leftptr = left->leftptr;
		new_left->rightptr = right->leftptr;
		new_right->leftptr = left->rightptr;
		new_right->rightptr = right->rightptr;
		//free(root);
		//free(left);
		//free(right);
		new_left = ROBDDmain(new_left);
		new_right = ROBDDmain(new_right);
		if (new_left->type == 0 && new_right->type == 0 && new_left->value == new_right->value)
		{
			new_left->fatherptr = root->fatherptr;
			//free(new_root);
			//free(new_left);
			return new_left;
		}
		new_root->leftptr = new_left;
		new_root->rightptr = new_right;
		return new_root;
	}//如果左右节点为一样的命题节点
	if (left->type == 2 && (right->type == 0 || left->value < right->value))
	{
		BDDNode* new_left, * new_right, * new_root;
		new_root = initBDDNode(2, left->value, root->fatherptr);
		new_left = initBDDNode(1, root->value, new_root);
		new_right = initBDDNode(1, root->value, new_root);

		new_left->leftptr = left->leftptr;
		new_left->rightptr = root->rightptr;
		new_right->leftptr = left->rightptr;
		new_right->rightptr = root->rightptr;
		//free(root);
		//free(left);
		//free(right);
		new_left = ROBDDmain(new_left);
		new_right = ROBDDmain(new_right);
		if (new_left->type == 0 && new_right->type == 0 && new_left->value == new_right->value)
		{
			new_left->fatherptr = root->fatherptr;
			//free(new_root);
			//free(new_left);
			return new_left;
		}
		new_root->leftptr = new_left;
		new_root->rightptr = new_right;

		return new_root;
	}//左侧节点更小
	if (right->type == 2 && (left->type == 0 || right->value < left->value))
	{
		BDDNode* new_left, * new_right, * new_root;
		new_root = initBDDNode(2, right->value, root->fatherptr);
		new_left = initBDDNode(1, root->value, new_root);
		new_right = initBDDNode(1, root->value, new_root);

		new_left->leftptr = root->leftptr;
		new_left->rightptr = right->leftptr;
		new_right->leftptr = root->leftptr;
		new_right->rightptr = right->rightptr;
		//free(root);
		//free(left);
		//free(right);
		new_left = ROBDDmain(new_left);
		new_right = ROBDDmain(new_right);
		if (new_left->type == 0 && new_right->type == 0 && new_left->value == new_right->value)
		{
			new_left->fatherptr = root->fatherptr;
			//free(new_root);
			//free(new_left);
			return new_left;
		}
		new_root->leftptr = new_left;
		new_root->rightptr = new_right;
		return new_root;
	}//
	exit(-2);
}

void BDDGraph(string output, BDDNode* root)
{
	char* out;
	output += ".dot";
	out = (char*)output.c_str();
	out_path.open(out, ios::ate);
	//if (out_path.is_open()) cout << "open success1" << endl;
	out_path << "digraph G{" << endl;
	out_path.close();
	BDDDraw(out, root, 0);
	out_path.open(out, ios::app);
	//if (out_path.is_open()) cout << "open success3" << endl;
	out_path << "}" << endl;
	out_path.close();
	return;

}

int BDDDraw(char* output, BDDNode* root, int node_cnt)
{
	out_path.open(output, ios::app);
	//if (out_path.is_open()) cout << "open success2" << endl;
	if (root->name == -1)
	{
		root->name = node_cnt++;
		if (root->type == 2)
		{
			out_path << root->name << "[label = \" " << root->value << "\"]" << endl;
			out_path.close();
			node_cnt = BDDDraw(output, root->leftptr, node_cnt);
			node_cnt = BDDDraw(output, root->rightptr, node_cnt);
			out_path.open(output, ios::app);
			out_path << root->name << "->" << root->leftptr->name << "[color = \"#242038\"];" << endl;
			out_path << root->name << "->" << root->rightptr->name << "[color = \"#FF595E\"];" << endl;
		}
		else if (root->type == 0)
		{
			if (root->value == 0)
			{
				out_path << root->name << "[label = \" False \"]" << endl;
			}
			else
			{
				
				out_path << root->name << "[label = \" True \"]" << endl;
			}
		}
		else
		{
			//等待再实现
		}
		
	}
	out_path.close();
	return node_cnt;
}