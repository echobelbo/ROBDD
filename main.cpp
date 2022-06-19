#include<iostream>
#include"ROBDD.h"
#include<string>


int main(void)
{
	string input;
	cin >> input;
	BDDNode* root = GenerateBDDtree(input);
	printBDDNode(root, "");
	root = ROBDDmain(root);
	printBDDNode(root, "");
	BDDGraph("a", root);
	return 0;
}

