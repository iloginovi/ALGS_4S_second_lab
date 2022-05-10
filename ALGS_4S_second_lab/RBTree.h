#ifndef RBTREE_H
#define RBTREE_H

#include "stack.h"
#include "iterator.h"
#include "list.h"
#include "pair.h"

using namespace std;

template <class T1, class T2>
class RBTree
{
private:
	enum class color { RED = 0, BLACK = 1 };
	class Node
	{
	private:
		Node* parent;
		Node* left;
		Node* right;

		T1* key;
		T2 data;

		color color;
	public:
		friend class RBTree;

		friend class RBTreeIterator;


		Node(const T1& key, const T2& data) : Node()
		{
			this->key = new T1[1];
			*this->key = key;
			this->data = data;
		}

		Node()
		{
			left = nullptr;
			right = nullptr;
			parent = nullptr;
			key = nullptr;
			color = color::RED;
		}

		~Node()
		{
			if (key != nullptr)
				delete[] key;
		}
	};

	Node* root;

	Node* Nil;
public:
	RBTree()
	{
		Nil = new Node();
		Nil->color = color::BLACK;
		root = nullptr;
	}

	void LeftSwitch(Node* X)
	{
		Node* Y = X->right;

		Node* B = Y->left;

		X->right = B;

		if (B != Nil)
		{
			B->parent = X;
		}

		Node* P = X->parent;
		Y->parent = P;

		if (P == nullptr)
		{
			this->root = Y;
		}
		else if (X == P->left)
		{
			P->left = Y;
		}
		else
		{
			P->right = Y;
		}

		Y->left = X;
		X->parent = Y;
	}

	void RightSwitch(Node* X)
	{
		Node* Y = X->left;

		Node* B = Y->right;

		X->left = B;

		if (B != Nil)
		{
			B->parent = X;
		}

		Node* P = X->parent;
		Y->parent = P;

		if (P == nullptr)
		{
			this->root = Y;
		}
		else if (X == P->right)
		{
			P->right = Y;
		}
		else
		{
			P->left = Y;
		}

		Y->right = X;
		X->parent = Y;
	}

	Node* ParentNodeToInsert(T1 key)
	{
		Node* currNode = root;

		if (key == *currNode->key)
		{
			return Nil;
		}


		while (true)
		{
			if (key > *currNode->key)
			{
				if (currNode->right != Nil)
				{
					if (*currNode->right->key == key)
						return currNode;
					currNode = currNode->right;
				}
				else
					return currNode;
			}
			else
			{
				if (currNode->left != Nil)
				{
					if (*currNode->left->key == key)
						return currNode;
					currNode = currNode->left;
				}
				else return currNode;
			}
		}
	}

	void Add(const T1& key, const T2& data)
	{
		if (root == nullptr)
		{
			Node* newNode = new Node(key, data);

			root = newNode;
			root->color = color::BLACK;

			newNode->left = Nil;
			newNode->right = Nil;
		}
		else
		{
			Node* parentNode = ParentNodeToInsert(key);

			if (parentNode == Nil)
			{
				root->data += data;
				return;
			}


			if (parentNode->left != Nil && *parentNode->left->key == key)
			{
				parentNode->left->data += data;
				return;

			}
			else if (parentNode->right != Nil && *parentNode->right->key == key)
			{
				parentNode->right->data += data;
				return;
			}

			Node* newNode = new Node(key, data);

			if (key < *parentNode->key)
				parentNode->left = newNode;
			else
				parentNode->right = newNode;

			newNode->parent = parentNode;
			newNode->left = Nil;
			newNode->right = Nil;

			newNode->color = color::RED;

			FixInsert(newNode);
		}
	}



	void Insert(const T1& key, const T2& data)
	{
		if (root == nullptr)
		{
			Node* newNode = new Node(key, data);

			root = newNode;
			root->color = color::BLACK;

			newNode->left = Nil;
			newNode->right = Nil;
		}
		else
		{
			Node* parentNode = ParentNodeToInsert(key);

			if (parentNode == Nil)
			{
				throw invalid_argument("Key already exists");
			}

			if (parentNode->left != Nil && *parentNode->left->key == key)
			{
				throw invalid_argument("Key already exists");

			}
			else if (parentNode->right != Nil && *parentNode->right->key == key)
			{
				throw invalid_argument("Key already exists");
			}

			Node* newNode = new Node(key, data);

			if (key < *parentNode->key)
				parentNode->left = newNode;
			else
				parentNode->right = newNode;

			newNode->parent = parentNode;
			newNode->left = Nil;
			newNode->right = Nil;

			newNode->color = color::RED;

			FixInsert(newNode);
		}
	}

	void FixInsert(Node* newNode)
	{
		while (newNode->parent->color == color::RED)
		{
			Node* p = newNode->parent;
			Node* Gp = p->parent;

			if (p == Gp->left)
			{
				// Case I
				if (Gp->right->color == color::RED)
				{
					Gp->left->color = color::BLACK;
					Gp->right->color = color::BLACK;
					Gp->color = color::RED;
				}
				else // Case II
				{
					if (newNode == p->right)
					{
						newNode = p;
						LeftSwitch(newNode);
						p = newNode->parent;
						Gp = p->parent;
					}
					// Case III
					p->color = color::BLACK;
					Gp->color = color::RED;
					RightSwitch(Gp);
				}
			}
			else
			{
				// Case I
				if (Gp->left->color == color::RED)
				{
					Gp->left->color = color::BLACK;
					Gp->right->color = color::BLACK;
					Gp->color = color::RED;
					newNode = Gp;
				}
				else // Case II
				{
					if (newNode == p->left)
					{
						newNode = p;
						RightSwitch(newNode);
						p = newNode->parent;
						Gp = p->parent;
					}

					// Case III
					p->color = color::BLACK;
					Gp->color = color::RED;
					LeftSwitch(Gp);
				}
			}

			if (newNode == root)
				break;
		}
		// Case IV
		root->color = color::BLACK;
	}

	void Delete(const T1& key)
	{
		Node* nodeToDelete = FindNode(key);

		if (nodeToDelete != Nil)
		{
			color originalColor = nodeToDelete->color;

			Node* X = nullptr;

			if (nodeToDelete->left == Nil)
			{
				X = nodeToDelete->right;
				NodeTransplant(nodeToDelete, X);
			}
			else if (nodeToDelete->right == Nil)
			{
				X = nodeToDelete->left;
				NodeTransplant(nodeToDelete, X);
			}
			else
			{
				Node* Y = FindMinimum(nodeToDelete->right);

				originalColor = Y->color;
				X = Y->right;

				if (Y->parent == nodeToDelete)
				{
					X->parent = Y;
				}
				else
				{
					NodeTransplant(Y, Y->right);
					Y->right = nodeToDelete->right;
					Y->right->parent = Y;
				}

				NodeTransplant(nodeToDelete, Y);

				Y->color = originalColor;
				Y->left = nodeToDelete->left;
				Y->left->parent = Y;
				Y->color = nodeToDelete->color;
			}

			delete nodeToDelete;

			if (originalColor == color::BLACK)
			{
				FixDelete(X);
			}
		}
		else
		{
			throw invalid_argument("Node does not exist.");
		}
	}

	void FixDelete(Node* X)
	{
		Node* W;
		while (X != root && X->color == color::BLACK)
		{
			if (X == X->parent->left)
			{
				W = X->parent->right;
				if (W->color == color::RED)
				{
					W->color = color::BLACK;
					X->parent->color = color::RED;
					LeftSwitch(X->parent);
					W = X->parent->right;
				}

				if (W->left->color == color::BLACK && W->right->color == color::BLACK)
				{
					W->color = color::RED;
					X = X->parent;
				}
				else
				{
					if (W->right->color == color::BLACK) {
						W->left->color = color::BLACK;
						W->color = color::RED;
						RightSwitch(W);
						W = X->parent->right;
					}

					W->color = X->parent->color;
					X->parent->color = color::BLACK;
					W->right->color = color::BLACK;
					LeftSwitch(X->parent);
					X = this->root;
				}
			}
			else
			{
				W = X->parent->left;
				if (W->color == color::RED) {
					W->color = color::BLACK;
					X->parent->color = color::RED;
					RightSwitch(X->parent);
					W = X->parent->left;
				}

				if (W->left->color == color::BLACK && W->right->color == color::BLACK)
				{
					W->color = color::RED;
					X = X->parent;
				}
				else
				{
					if (W->left->color == color::BLACK)
					{
						W->right->color = color::BLACK;
						W->color = color::RED;
						LeftSwitch(W);
						W = X->parent->left;
					}

					W->color = X->parent->color;
					X->parent->color = color::BLACK;
					W->left->color = color::BLACK;
					RightSwitch(X->parent);
					X = this->root;
				}
			}
		}
		X->color = color::BLACK;
	}

	Node* FindNode(const T1& key)
	{
		Node* currNode = root;

		while (currNode != Nil)
		{
			if (*currNode->key == key)
				return currNode;

			if (*currNode->key < key)
			{
				currNode = currNode->right;
			}
			else if (*currNode->key > key)
			{
				currNode = currNode->left;
			}
		}
		return Nil;
	}

	void NodeTransplant(Node* X, Node* Y)
	{
		if (X->parent == nullptr)
		{
			root = Y;
		}
		else if (X == X->parent->left)
		{
			X->parent->left = Y;
		}
		else
		{
			X->parent->right = Y;
		}
		Y->parent = X->parent;
	}

	Node* FindMinimum(Node* node)
	{
		while (node->left != Nil)
		{
			node = node->left;
		}
		return node;
	}

	T2 FindData(const T1& key)
	{
		Node* node = FindNode(key);
		if (node == Nil)
		{
			throw invalid_argument("Node does not exist.");
		}
		else
		{
			return node->data;
		}
	}

	T1 GetKey(Node* node)
	{
		return *node->key;
	}

	T2 GetData(Node* node)
	{
		return node->data;
	}

	~RBTree()
	{
		auto itr = create_iterator();

		while (itr->has_next())
		{
			Node* temp = itr->current;
			itr->next();
			delete temp;
		}
		delete itr;

		delete Nil;
	}

	class RBTreeIterator : public Iterator<Pair<T1, T2>>
	{
	public:
		friend class RBTree;

		Stack<Node*>* stack;

		Node* current;

		RBTreeIterator(Node* root)
		{
			current = root;
			stack = new Stack<Node*>();
		}

		~RBTreeIterator()
		{
			delete stack;
		}

		bool has_next()
		{
			return current != nullptr;
		}

		Pair<T1, T2> next()
		{
			Node* temp = current;

			Pair<T1, T2> pair = makepair(*temp->key, temp->data);

			if (current->right->key != nullptr)
			{
				stack->push(current->right);
			}

			if (current->left->key != nullptr)
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

			return pair;
		}

	};

	RBTreeIterator* create_iterator()
	{
		return new RBTreeIterator(root);
	}

};

#endif