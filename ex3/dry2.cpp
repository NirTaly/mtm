#include <vector>
#include <iostream>

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

class A 
{
public:
	std::vector<int*> operator=(const std::vector<int*>& vec)
	{
		std::vector<int*> tmp;
		for (auto p_i : vec)
		{
			tmp.push_back(new int(*p_i));
		}
		
		
	}

	std::vector<int*> values;
	void add(int x) { values.push_back(new int(x)); }
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