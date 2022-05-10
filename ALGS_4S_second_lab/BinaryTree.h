#ifndef BINARYTREEH
#define BINARYTREEH

#include <iostream>

#include "iterator.h"
#include "stack.h"
#include "HufTree.h"


bool operator < (const HuffmanNode& node1, const HuffmanNode& node2)
{
	return node1.count < node2.count;
}

bool operator > (const HuffmanNode& node1, const HuffmanNode& node2)
{
	return node1.count > node2.count;
}

bool operator == (const HuffmanNode& node1, const HuffmanNode& node2)
{
	return node1.count == node2.count;
}

template <class T>
class BinaryHeap
{
private:
	class Node
	{
	public:
		friend class BinaryHeap;
		friend class HeapIterator;

		Node* parent;
		Node* left;
		Node* right;

		T data;

		Node(T data)
		{
			this->left = nullptr;
			this->right = nullptr;
			this->parent = nullptr;
			this->data = data;
		}
	};



	void Heapify(Node* node)
	{
		if (node->parent == nullptr)
		{
			siftDown(node);
		}
		else
			if (node->data < node->parent->data)
			{
				//cout << "PROC1" << endl;
				siftUp(node);
			}
			else
			{
				//cout << "PROC2" << endl;
				siftDown(node);
			}
	}

	void siftDown(Node* node)
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			return;
		}

		Node* maxNode = node; //node to swap with

		if (node->right == nullptr)  //Means LEFT is not NULL
		{
			if (node->data > node->left->data)
			{
				maxNode = node->left;
			}
			else
			{
				return;
			}
		}

		if (node->left != nullptr && node->right != nullptr)
		{
			if (node->data > node->right->data || node->data > node->left->data)
			{
				if (node->left->data < node->right->data)
				{
					maxNode = node->left;
				}
				else
				{
					maxNode = node->right;
				}
			}
			else
			{
				return;
			}
		}

		T temp = node->data;
		node->data = maxNode->data;
		maxNode->data = temp;
		siftDown(maxNode);

	}

	void siftUp(Node* node)
	{
		if (node->parent == nullptr)
		{
			return;
		}
		else
		{
			if (node->data < node->parent->data)
			{
				T temp = node->data;
				node->data = node->parent->data;
				node->parent->data = temp;
				siftUp(node->parent);
			}
			else
			{
				return;
			}
		}
	}

	Node* findParentOfLast(int lastLineSize)
	{
		Node* parentNode = root;

		int tempLastLineSize = lastLineSize;
		int tempLastLineNodesCount = lastLineNodesCount;

		if (lastLineNodesCount > lastLineSize)
		{
			tempLastLineSize *= 2;
		}

		while (tempLastLineSize != 2)
		{
			if (tempLastLineNodesCount <= tempLastLineSize / 2)
			{
				parentNode = parentNode->left;
				tempLastLineSize /= 2;
			}
			else
			{
				parentNode = parentNode->right;
				tempLastLineNodesCount = tempLastLineNodesCount - tempLastLineSize / 2;
				tempLastLineSize /= 2;
			}
		}

		return parentNode;
	}
public:
	Node* root;
	int lastLineNodesCount;

	int height;

	BinaryHeap()
	{
		root = nullptr;
		height = 0;
		lastLineNodesCount = 0;
	}

	~BinaryHeap()
	{
		Iterator<Node*>* newiterator = create_iterator();
		while (newiterator->has_next())
		{
			Node* next = newiterator->next();
			delete next;
		}
		delete newiterator;
	}

	bool empty()
	{
		return root == nullptr;
	}

	void insert(T data)
	{
		Node* newNode = new Node(data);

		if (root == nullptr)
		{
			root = newNode;
			height = 1;
			lastLineNodesCount = 1;


		}
		else
		{
			Node* parentNode = root;

			int lastLineSize = 1;

			for (int i = 0; i < height - 1; i++) //Counting last line MAX size
			{
				lastLineSize *= 2;
			}

			lastLineNodesCount++;
			if (lastLineNodesCount > lastLineSize)
			{
				height++;
				lastLineNodesCount = 1;
				lastLineSize *= 2;
			}

			parentNode = findParentOfLast(lastLineSize);

			if (parentNode->left == nullptr)
			{
				parentNode->left = newNode;
				newNode->parent = parentNode;
			}
			else
			{
				parentNode->right = newNode;
				newNode->parent = parentNode;
			}



			Heapify(newNode);

			return;
		}
	}

	Node* findNode(T data)
	{
		auto newiterator = create_iterator();
		while (newiterator->has_next())
		{
			Node* next = newiterator->next();
			if (next->data == data)
			{
				delete newiterator;
				return next;
			}
		}
		delete newiterator;
		throw invalid_argument("Element not found!");
	}

	void remove(T data)
	{
		Node* nodeToRemove = findNode(data);

		if (height == 1)
		{
			delete root;
			root = nullptr;
			height = 0;
			lastLineNodesCount = 0;
			return;
		}

		int lastLineSize = 1;

		for (int i = 0; i < height - 1; i++) //Counting last line MAX size
		{
			lastLineSize *= 2;
		}

		Node* lastNode = findParentOfLast(lastLineSize);

		if (lastNode->right != nullptr) //Cut last node from tree
		{
			lastNode = lastNode->right;
			lastNode->parent->right = nullptr;
		}
		else
		{
			lastNode = lastNode->left;
			lastNode->parent->left = nullptr;
		}

		lastLineNodesCount--; //height change
		if (lastLineNodesCount == 0)
		{
			height--;
			lastLineNodesCount = 1;
			for (int i = 0; i < height - 1; i++)
			{
				lastLineNodesCount *= 2;
			}
		}

		if (lastNode == nodeToRemove)
		{
			delete lastNode;
		}
		else
		{
			T temp = lastNode->data;
			lastNode->data = nodeToRemove->data;
			nodeToRemove->data = temp;

			delete lastNode;

			Heapify(nodeToRemove);
		}
	}

	T getRoot()
	{
		return root->data;
	}

	T pop()
	{
		T temp = getRoot();
		remove(temp);
		return temp;
	}

	void printHeap()
	{
		Iterator<Node*>* newiterator = create_iterator();
		while (newiterator->has_next())
		{
			cout << "Node: ";
			Node* next = newiterator->next();
			cout << next->data << endl;
		}
	}


	class HeapIterator : public Iterator<Node*>
	{
	public:
		friend class BinaryHeap;

		HeapIterator(Node* root)
		{
			current = root;
			stack = new Stack<Node*>();
		}

		~HeapIterator()
		{
			delete stack;
		}

		bool has_next()
		{
			return current != nullptr;
		}

		Node* next()
		{
			if (!has_next())
			{
				throw out_of_range("No more elements");
			}

			Node* temp = current;



			if (current->right != nullptr)
			{
				stack->push(current->right);
			}

			if (current->left != nullptr)
			{
				current = current->left;
			}
			else
			{
				if (!stack->empty())
				{
					current = stack->top();
					stack->pop();
				}
				else
				{
					current = nullptr;
				}
			}

			return temp;
		}
	private:
		Stack<Node*>* stack;

		Node* current;
	};

	HeapIterator* create_iterator()
	{
		return new HeapIterator(root);
	}
};

#endif
