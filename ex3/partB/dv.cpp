#include "dv.h"

namespace mtm
{
	template <typename T>
	DynamicVector<T>::DynamicVector(size_t size) throw(std::bad_alloc) : 
			m_dv(new T[size]), m_length(0), m_capacity(size) { }
		
	template <typename T>
	DynamicVector<T>::~DynamicVector()
	{
		delete[] m_dv;
	}
	
	template <typename T>
	DynamicVector<T>::DynamicVector(const DynamicVector& other) throw(std::bad_alloc): 
		m_dv(NULL), m_length(other.length()), m_capacity(0)
	{
		size_t new_length = other.m_length > 0 ? other.m_length * CONST : CONST;

		m_dv = new T[new_length];
		m_capacity = new_length;

		for(int i = 0; i < other.length(); i ++) {
			m_dv[i] = other.m_dv[i];
		}
	}

	template <typename T>
	void DynamicVector<T>::insert(T element) throw(std::bad_alloc)
	{
		if(m_length == m_capacity)
		{
			DynamicVector tmp(*this);

			swap(tmp);
		}
		
		m_dv[m_length] = element;
		m_length++;
	}
	
	template <typename T>
	void DynamicVector<T>::pop()
	{
		if (m_length)
		{
			m_length--;
		}
	}
	
	template <typename T>
	T* DynamicVector<T>::last() const
	{
		if (m_length)
		{
			return (m_dv + (m_length - 1));
		}
		return (NULL);
	}

	template <typename T>
	void DynamicVector<T>::remove(T element)
	{
		for (int i = 0; i < m_length; i++)
		{
			if (m_dv[i] == element)
			{
				if (i != (m_length - 1))
				{
					m_dv[i] = m_dv[m_length-1];
				}
				
				m_length--;
			}
		}
	}

	template <typename T>
	bool DynamicVector<T>::isIn(T element) const
	{
		for (int i = 0; i < m_length; i++)
		{
			if (m_dv[i] == element)
			{
				return true;
			}
		}

		return false;
	}
	
	template <typename T>
	bool DynamicVector<T>::isEmpty() const
	{
		return (m_length == 0);
	}
	
	template <typename T>
	size_t DynamicVector<T>::length() const
	{
		return m_length;
	}

	template <typename T>
	size_t DynamicVector<T>::capacity() const
	{
		return m_capacity;
	}

	template class DynamicVector<size_t>;
}