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
	string name;
	//type = 1 Ϊ������ڵ�, value=0���Ͳ�����value=1���������value=2���̺�������value = 3���ȼ۲���
	//type = 2 Ϊ����ڵ㣬valueΪ������
	//type = 0 ΪҶ�ӽڵ㣬valueΪ1��0

};

BDDNode* initBDDNode(int mode, int value, BDDNode* father);
void printBDDNode(BDDNode* root, string before);
BDDNode* GenerateBDDtree(string input);
BDDNode* ROBDDmain(BDDNode* root);