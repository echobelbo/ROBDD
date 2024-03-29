#pragma once

#include<string>
using namespace std;
class BDDNode
{
public:
	int type;
	int value;
	BDDNode* leftptr;
	BDDNode* rightptr;
	BDDNode* fatherptr;
	int name;
	//type = 1 为运算符节点, value=0：和操作；value=1：或操作；value=2，蕴含操作；value = 3，等价操作
	//type = 2 为命题节点，value为命题编号
	//type = 0 为叶子节点，value为1或0

};

BDDNode* initBDDNode(int mode, int value, BDDNode* father);
void printBDDNode(BDDNode* root, string before);
BDDNode* GenerateBDDtree(string input);
BDDNode* ROBDDmain(BDDNode* root);
void BDDGraph(string output, BDDNode* root);
int BDDDraw(char* output, BDDNode* root, int node_cnt);