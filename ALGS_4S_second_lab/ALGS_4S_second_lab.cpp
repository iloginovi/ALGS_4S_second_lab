#include <iostream>
#include <string>
#include "pair.h"
#include "map.h"
#include "BinaryTree.h"
#include "HufTree.h"
using namespace std;





HuffmanNode::HuffmanNode()
{
	this->symbols = "NONE";
	this->count = -1;

	code = false;
	parent = nullptr;
	left = nullptr;
	right = nullptr;
}

HuffmanNode::HuffmanNode(string symbols, int count) : HuffmanNode()
{
	this->symbols = symbols;
	this->count = count;
}

HuffmanTree::HuffmanTree()
{
	root = nullptr;
	leafsList = new List<HuffmanNode*>();
}
inline string const BoolToString(bool b)
{
	return b ? "1" : "0";
}

void PrintCodes(Map<char, List<bool>*>& codesMap)
{
	auto codes_itr = codesMap.create_iterator();

	while (codes_itr->has_next())
	{
		Pair<char, List<bool>*> newPair = codes_itr->next();
		cout << newPair.first << ": ";

		auto list_itr = newPair.second->create_iterator();

		while (list_itr->has_next())
		{
			cout << BoolToString(list_itr->next());
		}

		cout << endl;
	}
}

void PrintFreqency(Map<char, int>& symbolsMap)
{
	auto sym_itr = symbolsMap.create_iterator();

	while (sym_itr->has_next())
	{
		Pair<char, int> newPair = sym_itr->next();
		cout << newPair.first << ": " << newPair.second << endl;
	}
}

string bitSequanceToString(List<bool>& bitLine)
{
	string line;
	auto list_itr = bitLine.create_iterator();
	while (list_itr->has_next())
		line += BoolToString(list_itr->next());
	delete list_itr;
	return line;
}

List<bool> EncodeByHuffman(string line, Map<char, List<bool>*>& haffmanCode)
{
	List<bool>* encodedLine = new List<bool>();

	for (char character : line)
	{
		List<bool>* code = haffmanCode.Find(character);

		auto list_itr = code->create_iterator();
		while (list_itr->has_next())
		{
			encodedLine->push_back(list_itr->next());
		}
	}
	return *encodedLine;
}

string DecodeByHaffman(List<bool>& encodedLine, HuffmanTree& tree)
{
	string decodedLine;
	HuffmanNode* current = tree.root;

	auto encodedLineBitIterator = encodedLine.create_iterator();
	while (encodedLineBitIterator->has_next())
	{
		bool bit = encodedLineBitIterator->next();

		if (bit)
		{
			current = current->left;

			if (current->left == nullptr)
			{
				decodedLine += current->symbols;
				current = tree.root;
			}
			else if (!(encodedLineBitIterator->has_next()))
			{
				throw out_of_range("Appropriate decoding sequance was not found");
			}
		}
		else
		{
			current = current->right;

			if (current->left == nullptr)
			{
				decodedLine += current->symbols;
				current = tree.root;
			}
			else if (!(encodedLineBitIterator->has_next()))
			{
				throw out_of_range("Appropriate decoding sequance was not found");
			}
		}
	}
	return decodedLine;
}

Map<char, List<bool>*> CreateHuffmanCode(HuffmanTree& tree)
{
	auto bufferItr = tree.leafsList->create_iterator();

	Map<char, List<bool>*>* haffcode = new Map<char, List<bool>*>();

	while (bufferItr->has_next())
	{
		HuffmanNode* current = bufferItr->next();

		List<bool>* symbolCode = new List<bool>();

		char currSymbol = (current->symbols)[0];

		while (current->parent != nullptr)
		{
			symbolCode->push_front(current->code);

			current = current->parent;
		}

		haffcode->Insert(currSymbol, symbolCode);
	}

	return *haffcode;
}

HuffmanTree buildHuffmanTree(Map<char, int>& symbolsMap)
{
	HuffmanTree* tree = new HuffmanTree();

	BinaryHeap<HuffmanNode>* heap = new BinaryHeap<HuffmanNode>();

	auto itr = symbolsMap.create_iterator();
	while (itr->has_next())
	{
		Pair<char, int> newPair = itr->next();
		HuffmanNode newNode(string(1, newPair.first), newPair.second);
		heap->insert(newNode);
	}

	List<HuffmanNode*>* leafsList = new List<HuffmanNode*>();

	while (heap->height != 1)
	{
		HuffmanNode n1 = heap->pop();
		HuffmanNode n2 = heap->pop();

		HuffmanNode* minNode1 = &n1;
		HuffmanNode* minNode2 = &n2;

		if (minNode1->left == nullptr)
		{
			HuffmanNode* realminNode1 = new HuffmanNode(minNode1->symbols, minNode1->count);
			leafsList->push_front(realminNode1);
			minNode1 = realminNode1;
		}
		else
		{
			minNode1 = minNode1->left->parent;
		}

		if (minNode2->left == nullptr)
		{
			HuffmanNode* realminNode2 = new HuffmanNode(minNode2->symbols, minNode2->count);
			leafsList->push_front(realminNode2);
			minNode2 = realminNode2;
		}
		else
		{
			minNode2 = minNode2->left->parent;
		}

		string combinedSymbols = minNode1->symbols + minNode2->symbols;
		int combinedCount = minNode1->count + minNode2->count;

		HuffmanNode* newNode = new HuffmanNode(combinedSymbols, combinedCount);
		tree->root = newNode;

		minNode1->code = 0;
		minNode2->code = 1;

		newNode->right = minNode1;
		newNode->left = minNode2;


		minNode1->parent = newNode;
		minNode2->parent = newNode;

		heap->insert(*newNode);
	}

	delete heap;
	tree->leafsList = leafsList;
	return *tree;
}

int main() {
	string quote;
	quote = "Second test string, a bit longer one.";
	//"I know, that i know nothing." - Socrates.
	//"Success does not consist in never making mistakes but in never making the same one a second time." - George Bernard Shaw.
	//"While we are postponing, life speeds by." - Lucius Annaeus Seneca.
	cout << quote << endl;

	Map<char, int> symbolsMap;
	for (char character : quote)
		symbolsMap.Add(character, 1);

	cout << "\nFreqency:" << endl;
	PrintFreqency(symbolsMap);

	HuffmanTree huffmantree;
	huffmantree = buildHuffmanTree(symbolsMap);

	Map<char, List<bool>*> haffmanCode = CreateHuffmanCode(huffmantree);

	cout << "\nCodes:" << endl;
	PrintCodes(haffmanCode);

	List<bool> encodedLine = EncodeByHuffman(quote, haffmanCode);
	cout << "\nEncoded line:" << endl;
	cout << bitSequanceToString(encodedLine);

	try
	{
		string decodedLine = DecodeByHaffman(encodedLine, huffmantree);
		cout << "\n\nDecoded line: " << decodedLine << endl;
	}
	catch (out_of_range& expection)
	{
		cout << expection.what() << endl;
	}

	int sizeBeforeEncode = quote.length() * sizeof(char) * 8;
	int sizeAferEncode = encodedLine.size;

	cout << "Size before encode: " << sizeBeforeEncode << " bits" << endl;
	cout << "Size after encode: " << sizeAferEncode << " bits" << endl;
	cout << "Compression coefficient: " << (sizeAferEncode * 1.0) / (sizeBeforeEncode * 1.0) << endl;


	auto itr = haffmanCode.create_iterator();
	while (itr->has_next())
	{
		auto pair = itr->next();
		delete pair.second;
	}
	return 0;
}