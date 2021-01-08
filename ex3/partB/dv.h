#ifndef __DV_H__
#define __DV_H__

#include <iostream>      // size_t


using std::size_t;

namespace mtm
{
	template <typename T>
	class DynamicVector
	{
		class Proxy;

	public:
		DynamicVector(size_t size = 10)	throw(std::bad_alloc);
		~DynamicVector();
		DynamicVector(const DynamicVector& other) throw(std::bad_alloc);
		DynamicVector& operator=(const DynamicVector& other) = delete;

		Proxy operator[](T element) throw(std::bad_alloc);
		
		void insert(T element) throw(std::bad_alloc);
		
		void pop();
		
		// return NULL if isEmpty(), else pointer to element in dv
		T* last() const;
		
		void remove(T element);
		
		bool isIn(T element) const;
		
		bool isEmpty() const;
		
		size_t length() const;
		size_t capacity() const;

	private:
		T* m_dv;
		size_t m_length;
		size_t m_capacity;
		
		void swap(DynamicVector& other)
		{
			T* tmp_dv = m_dv;
			m_dv = other.m_dv;
			other.m_dv = tmp_dv;

			m_capacity = other.capacity();
			m_length = other.length();
		}
		
		const int CONST = 2;
	};

	/**
	 * @brief Proxy class for operator[] purposes
	 */
	template<typename T>
    class DynamicVector<T>::Proxy
    {
    public:
        Proxy(DynamicVector& dv, T element): m_dv(dv), m_element(element) { }

        // Invoked when proxy is used to modify the value.
        Proxy& operator=(bool value)  // b1[1] = true;
        {
			if (value)
			{
				m_dv.insert(m_element);
			}
			else
			{
				m_dv.remove(m_element);
			}			

            return *this; 
		}

        // Invoked when proxy is used to read the value.
        operator bool() const // if (b1[1])...
        {
            return (m_dv.isIn(m_element));
        }

    private:
        DynamicVector& m_dv;
        T m_element;
    };

	template <typename T>
	typename DynamicVector<T>::Proxy DynamicVector<T>::operator[](T element) throw(std::bad_alloc)
	{
		return Proxy(*this, element);
	}



/*****************************************************************************/
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

} // mtm


#endif     /* __DV_H__ */
