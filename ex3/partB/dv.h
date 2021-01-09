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





} // mtm


#endif     /* __DV_H__ */
