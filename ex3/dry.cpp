#include <vector>
#include <iostream>

/*								PART A										 */
/*****************************************************************************/
class BadInput : public std::exception {};

template <class T>
std::vector<T> slice(std::vector<T> vec, int start, int step, int stop) throw(BadInput)
{   
    int vec_size = vec.size();
    if (start < 0 || start >= vec_size || vec_size < stop || stop < 0 || step <= 0)
	{
        throw(BadInput());
	}
	
    std::vector<T> new_vec;
    if(start >= stop)
    {
        return new_vec;
    }

    for(int i = start; i < stop; i+=step)
    {
        new_vec.push_back(vec[i]);
    }

    return new_vec;
}

/*								PART B										 */
/*****************************************************************************/
class A 
{
	class SharedPtr;
public:
	std::vector<SharedPtr> values;
	void add(int x) { values.push_back(SharedPtr(new int(x))); }
private:
	/**
	 * @brief RAII shared pointer class
	 */
	class SharedPtr
	{
	public:
		explicit SharedPtr(int* ptr = 0): m_ref_count(new size_t(1)), m_data(ptr) { }
		~SharedPtr()
		{
			Destroy();
		}
		SharedPtr(const SharedPtr& other): m_ref_count(other.m_ref_count), m_data(other.m_data)
		{
			(*m_ref_count)++;
		}
		SharedPtr& operator=(const SharedPtr& other)
		{
			SharedPtr temp(other);

			Destroy();

			m_data = temp.m_data;
			m_ref_count = temp.m_ref_count;
			(*m_ref_count)++;
			
			return *this;
		}

		int& operator*() 
		{ 
			return (*m_data); 
		}
		int* operator->() 
		{ 
			return (m_data); 
		}

	private:
		size_t* m_ref_count;
		int* m_data;
		
		inline void Destroy()
		{
			if (--(*m_ref_count) == 0)
			{
				delete m_ref_count;
				delete m_data;
			}
		}
	};
};

int main() 
{
	A a, sliced;
	
	a.add(0); a.add(1); a.add(2); a.add(3); a.add(4); a.add(5);
	
	sliced.values = slice(a.values, 1, 1, 4);
	
	*(sliced.values[0]) = 800;
	
	std::cout << *(a.values[1]) << std::endl;
	
	return 0;
}