#include <iostream>
#include <vector>
#include <stdexcept>

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



void print(std::vector<char> s)
{
	for (auto c : s)
	{
		std::cout << c;
	}
	std::cout << std::endl;
}

int main()
{
	std::vector<char> vec1 {'a', 'b', 'c', 'd', 'e'};

	print(slice(vec1, 0, 2, 4));	// returns vector with values a,c
	print(slice(vec1, 1, 1, 5));	// returns vector with values b,c,d,e

	try
	{
		print(slice(vec1, 0, 2, 4));
	}catch(const std::exception& e)	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		print(slice(vec1, 0, 2, 4));
	}catch(const std::exception& e)	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		print(slice(vec1, 0, 2, 4));
	}catch(const std::exception& e)	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		print(slice(vec1, 0, 2, 4));
	}catch(const std::exception& e)	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}
