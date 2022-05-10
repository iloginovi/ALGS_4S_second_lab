#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template <class T>
class Stack
{
private:
	class Node
	{
	public:
		Node(T node, Node* prev)
		{
			this->node = node;
			this->prev = prev;
		}

		Node(T node) : Node(node, nullptr) {}

		T node;
		Node* prev;
	};

	Node* head;
public:
	Stack()
	{
		head = nullptr;
	}

	~Stack()
	{
		while (!empty())
		{
			pop();
		}
	}

	void pop()
	{
		if (empty())
		{
			throw out_of_range("No more elements");
		}
		else
		{
			Node* temp = head;
			head = head->prev;
			delete temp;
		}
	}

	void push(T node)
	{
		Node* newNode = new Node(node, head);
		head = newNode;
	}

	bool empty()
	{
		return head == nullptr;
	}

	T top()
	{
		if (empty())
		{
			throw out_of_range("Empty");
		}
		else
		{
			return head->node;
		}
	}
};

#endif