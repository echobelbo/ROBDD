#include<iostream>
#include"ROBDD.h"
#include<string>


int main(void)
{
	string input;
	cin >> input;
	BDDNode* root = GenerateBDDtree(input);
	printBDDNode(root, "");
	return 0;
}