#ifndef __PQ_H__
#define __PQ_H__

#include "../partA/exceptions.h"

namespace mtm
{
	template <class T, class Compare>
	class PriorityQueue final
	{
	public:
		struct Node
		{
			Node()
			{
				m_element = nullptr;
				m_next = nullptr;
			}
			Node(const T& element, Node* next = nullptr) : m_element(new T(element)), m_next(next)
			{ }
			
			~Node()
			{
				delete m_element;
			}

			T* m_element;
			Node* m_next;
		};
		
		PriorityQueue();
		~PriorityQueue();
		PriorityQueue(const PriorityQueue<T, Compare>& other);
		PriorityQueue<T, Compare>& operator=(const PriorityQueue<T, Compare>& other) = delete;

		void push(const T& val);

		void remove(const T& out_val);
		
		void pop();
		
		Node* begin() const;
		Node* end() const;

		bool isIn(const T& element) const;

		bool isEmpty() const;
		size_t size() const;

		void print(std::ostream& os);
	private:
		Node m_end;
		Node m_start;
	};

	/******************************************************************************/
	/******************************************************************************/
	template <class T, class Compare>
	PriorityQueue<T,Compare>::PriorityQueue() 
	{ 
		m_start.m_next = &m_end;
	}
	
	template <class T, class Compare>
	PriorityQueue<T,Compare>::~PriorityQueue()
	{
		while (!isEmpty())
		{
			pop();
		}
	}

	template <class T, class Compare>
	PriorityQueue<T,Compare>::PriorityQueue(const PriorityQueue<T, Compare>& other)
	{
		m_start.m_next = &m_end;
		PriorityQueue<T,Compare>::Node* runner = other.m_start.m_next;
		for (; runner != &(other.m_end); runner = runner->m_next)
		{
			push(*(runner->m_element));
		}
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::push(const T& val)
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (Compare()(val, *(runner->m_next->m_element)))
			{
				break;
			}
		}

		PriorityQueue<T,Compare>::Node* inserted = new PriorityQueue<T,Compare>::Node(val,runner->m_next);
		
		runner->m_next = inserted;
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::remove(const T& out_val)
	{
		PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (*(runner->m_next->m_element) == out_val)
			{
				PriorityQueue<T,Compare>::Node* destroy_node = runner->m_next;
				runner->m_next = destroy_node->m_next;

				delete destroy_node;
				
				return;
			}
		}

		// throw (PQNotFound());
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::pop()
	{
		// if (isEmpty())
		// {
		// 	throw (PQEmpty());
		// }
		if (!isEmpty())
		{
			PriorityQueue<T,Compare>::Node* destroy_node = m_start.m_next;
			
			m_start.m_next = destroy_node->m_next;

			delete destroy_node;
		}
	}

	template <class T, class Compare>
	typename PriorityQueue<T,Compare>::Node* PriorityQueue<T,Compare>::begin() const
	{
		return m_start.m_next;
	}
	
	template <class T, class Compare>
	typename PriorityQueue<T,Compare>::Node* PriorityQueue<T,Compare>::end() const
	{
		return const_cast<PriorityQueue<T, Compare>::Node*>(&m_end);
	}

	template <class T, class Compare>
	bool PriorityQueue<T,Compare>::isIn(const T& element) const
	{
		const PriorityQueue<T,Compare>::Node* runner = &m_start;
		for (; runner->m_next != &m_end; runner = runner->m_next)
		{
			if (*(runner->m_next->m_element) == element)
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
		for (; runner != &m_end; runner = runner->m_next)
		{
			count++;
		}

		return count;
	}

	template <class T, class Compare>
	void PriorityQueue<T,Compare>::print(std::ostream& os)
	{
		for (auto runner = m_start.m_next; runner != &m_end; runner = runner->m_next)
		{
			os << *(runner->m_element) << std::endl;
		}
	}
}

#endif     /* __PQ_H__ */