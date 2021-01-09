#ifndef __PQ_H__
#define __PQ_H__

#include "../partA/exceptions.h"

namespace mtm
{
	template <class T, class Compare>
	class PriorityQueue final
	{
	public:
		PriorityQueue();
		~PriorityQueue();
		PriorityQueue(const PriorityQueue<T, Compare>& other) throw(std::bad_alloc);
		PriorityQueue<T, Compare>& operator=(const PriorityQueue<T, Compare>& other) = delete;

		void push(const T& val) throw(std::bad_alloc);

		void remove(const T& out_val) throw (PQNotFound);
		
		void pop() throw(PQEmpty);
		
		const T& begin();
		const T& end();
		const T& next();

		bool isIn(const T& element) const;

		bool isEmpty() const;
		size_t size() const;

	private:
	
		struct Node
		{
			Node(const T& element = 0, Node* next = nullptr) : m_element(element), m_next(next)
			{ }

			T m_element;
			Node* m_next;
		};

		Node m_end;
		Node m_start;
		Node* m_iter;
	};

	/******************************************************************************/
	/******************************************************************************/
	template <class T, class Compare>
	PriorityQueue<T,Compare>::PriorityQueue() : m_start(0, &m_end), m_iter(nullptr) { }
	
	template <class T, class Compare>
	PriorityQueue<T,Compare>::~PriorityQueue()
	{
		while (!isEmpty())
		{
			pop();
		}
	}

	template <class T, class Compare>
	PriorityQueue<T,Compare>::PriorityQueue(const PriorityQueue<T, Compare>& other) throw(std::bad_alloc)
	{
		for (T runner = other.begin(); runner != other.end(); runner = other.next())
		{
			push(runner);
		}
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::push(const T& val) throw(std::bad_alloc)
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (Compare(val, runner->m_next->m_element))
			{
				break;
			}
		}

		PriorityQueue<T,Compare>::Node* inserted = new PriorityQueue<T,Compare>::Node(val,runner->m_next);
		
		runner->m_next = inserted;
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::remove(const T& out_val)	throw (PQNotFound)
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (runner->m_next->m_element == out_val)
			{
				PriorityQueue<T,Compare>::Node* destroy_node = runner->m_next;
				runner->m_next = destroy_node->m_next;

				delete destroy_node;

				return;
			}
		}

		throw (PQNotFound());			//probably NotRegistered for student unregister
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::pop() throw(PQEmpty)
	{
		if (isEmpty())
		{
			throw (PQEmpty());
		}

		PriorityQueue<T,Compare>::Node* destroy_node = m_start.m_next;
		
		m_start.m_next = destroy_node->m_next;

		delete destroy_node;
	}

	template <class T, class Compare>
	const T& PriorityQueue<T,Compare>::begin()
	{
		m_iter = m_start.m_next;
		return m_start.m_next->m_element;
	}
	
	template <class T, class Compare>
	const T& PriorityQueue<T,Compare>::end()
	{
		return m_end.m_element;
	}

	template <class T, class Compare>
	const T& PriorityQueue<T,Compare>::next()
	{
		m_iter = m_iter->m_next;
		return m_iter->m_element;
	}

	template <class T, class Compare>
	bool PriorityQueue<T,Compare>::isIn(const T& element) const
	{
		const PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (runner->m_next->m_element == element)
			{
				return true;
			}
		}
		
		return false;
	}

	template <class T, class Compare>
	bool PriorityQueue<T,Compare>::isEmpty() const
	{
		return m_start.m_next == &m_end;
	}


	template <class T, class Compare>
	size_t PriorityQueue<T,Compare>::size() const
	{
		int count = 0;
		PriorityQueue<T,Compare>::Node* runner = m_start.m_next;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			count++;
		}

		return count;
	}

}

#endif     /* __PQ_H__ */