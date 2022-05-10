#ifndef HUFTREE_H
#define HUFTREE_H

#include <string.h>
#include "list.h"

class HuffmanNode
{
public:
	string symbols;
	int count;
	bool code;

	HuffmanNode* parent;
	HuffmanNode* left;
	HuffmanNode* right;

	HuffmanNode(string, int);

	HuffmanNode();
};

class HuffmanTree
{
public:
	HuffmanNode* root;

	List<HuffmanNode*>* leafsList;

	HuffmanTree();


};

#endif 