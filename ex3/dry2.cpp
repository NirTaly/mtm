#include <vector>
#include <iostream>

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
/******************************************************************************/
class A 
{
public:
	class Vector
	{
	public:
		
		Vector() = default;
		Vector(std::vector<int*> vec)
		{
			for (auto iter : vec) {
				m_vec.push_back(new int(*iter));
			}
		}
		~Vector()
		{
			for (auto iter : m_vec) {
				delete iter;
			}
		}
		Vector& operator=(const Vector& other)
		{
			for (auto iter : m_vec) {
				delete iter;
			}
			m_vec.clear();

			for (auto iter : other.m_vec) {
				m_vec.push_back(new int(*iter));
			}
			return *this;
		}
		int* operator[](int num)
		{
			return m_vec[num];
		}
		operator const std::vector<int*>&()
		{
			return m_vec;
		}

		std::vector<int*> m_vec;
	};

	Vector values;
	void add(int x) { values.m_vec.push_back(new int(x)); }
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