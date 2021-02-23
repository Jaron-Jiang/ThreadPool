#pragma once
template<class value_type>
class PriorityQueue
{
private:
	struct Data
	{
		value_type data;
		Data* next;
	};
	Data* front;
	Data* rear;
	int Length;
	value_type operator[](int index)
	{
		if (index == 0)
		{
			return front->data;
		}
		Data* p = front;
		for (int i = 0; i < index; i++)
		{
			p = p->next;
		}
		return p->data;
	}
	void operator=(PriorityQueue& _q)
	{
		clear();
		Data* p = _q.front;
		while (p)
		{
			push(p->data);
			p = p -> next;
		}
	}
public:
	PriorityQueue()
	{
		Length = 0;
		front = rear = NULL;
	}
	int size()
	{
		return Length;
	}
	void push(value_type& _value)
	{
		Data* s = new Data;
		s->data = _value;
		if (Length == 0 || front->data < _value)
		{
			s->next = front;
			front = s;
			if (Length == 0)
			{
				rear = front;
			}
		}
		else if (_value < rear->data)
		{
			rear->next = s;
			s->next = NULL;
			rear = s;
		}
		else
		{
			Data* father = front;
			Data* p = father->next;
			while (_value < p->data)
			{
				father = p;
				p = p->next;
			}
			father->next = s;
			s->next = p;
		}
		Length++;
	}
	void pop()
	{
		Data* p = front;
		front = front->next;
		delete p;
		Length--;
		if (Length == 0)
		{
			rear = NULL;
		}
	}
	value_type top()
	{
		return front->data;
	}
	bool empty()
	{
		return Length == 0;
	}
	bool clear()
	{
		while (Length)
		{
			pop();
		}
	}
};