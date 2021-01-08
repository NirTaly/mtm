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
		PriorityQueue(const PriorityQueue<T, Compare>& other) throw(std::bad_alloc);;
		PriorityQueue<T, Compare>& operator=(const PriorityQueue<T, Compare>& other) = delete;

		void push(const T& val) throw(std::bad_alloc);

		void remove(const T& out_val) throw (PQNotFound);
		
		void pop() throw(PQEmpty);
		
		const T* begin() const;
		const T* end() const;
		const T* next() const;

		bool isIn(const T& element) const;

		bool isEmpty() const;
		size_t size() const;
		
		friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& date);
	private:
		struct Node;

		struct Node
        {
			Node(Node* next = nullptr): m_element(nullptr), m_next(next) 
			{ }

            T* m_element;
            Node* m_next;
        };

		void destroy(Node* node)
		{
			delete node->m_element;
			delete node;
		}
		
		Node m_end;
		Node m_start;
		Node* m_iter;
	};

	/******************************************************************************/
	/******************************************************************************/
	template <class T, class Compare>
	PriorityQueue<T,Compare>::PriorityQueue() : m_start(&m_end), m_iter(nullptr) { }
	
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
		for (T* runner = other.begin(); runner != other.end(); runner = other.next())
		{
			push(*runner);
		}
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::push(const T& val) throw(std::bad_alloc)
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (Compare(val, *(runner->m_next)))
			{
				break;
			}
		}

		PriorityQueue<T,Compare>::Node* inserted = new PriorityQueue<T,Compare>::Node(runner->m_next);
		inserted->m_element = new T(val);
		runner->m_next = inserted;
		
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::remove(const T& out_val)	throw (PQNotFound)
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (*(runner->m_next) == out_val)
			{
				PriorityQueue<T,Compare>::Node* destroy_node = runner->m_next;
				runner->m_next = destroy_node->m_next;

				destroy(destroy_node);
			}
		}

		throw (PQNotFound())			//probably NotRegistered for student unregister
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::pop() throw(PQEmpty)
	{
		if (isEmpty())
		{
			throw (PQEmpty)
		}

		PriorityQueue<T,Compare>::Node* destroy_node = m_start.m_next;
		
		m_start.m_next = destroy_node->m_next;

		destroy(destroy_node);
	}

	template <class T, class Compare>
	const T* PriorityQueue<T,Compare>::begin() const
	{
		m_iter = m_start.m_next;
		return m_start.m_next->m_element;
	}
	
	template <class T, class Compare>
	const T* PriorityQueue<T,Compare>::end() const
	{
		return m_end.m_element;
	}

	template <class T, class Compare>
	const T* PriorityQueue<T,Compare>::next() const
	{
		m_iter = m_iter->m_next;
		return m_iter->m_element;
	}

	template <class T, class Compare>
	bool PriorityQueue<T,Compare>::isIn(const T& element) const
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (*(runner->m_next) == element)
			{
				return true;
			}
			return false;
		}
	}

	template <class T, class Compare>
	bool PriorityQueue<T,Compare>::isEmpty() const
	{
		return m_start.next == &m_end;
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
	
	template <class T, class Compare>
	std::ostream& operator<<(std::ostream& os, const PriorityQueue<T,Compare>& date)
	{
		PriorityQueue<T,Compare>::Node* runner = m_start.m_next;
		for (; runner != m_end; runner = rummer->m_next)
		{
			os << runner->m_element;
		}
	}
}

#endif     /* __PQ_H__ */