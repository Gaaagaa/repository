
#include <initializer_list>
#include <iostream>
#include <vector>

template< class T >
void print(std::initializer_list< T > list_values)
{
	for (const T & v : list_values)
	{
		std::cout << v << std::endl;
	}
}

int main(int argc, char * argv[])
{
	int x1(5.3);
	int x2 = 5.3;
	//! int x3{5.3};
	//! int x4 = {5.3};
	char c1{7};
	//! char c2{9999};
	std::vector< int > v1{ 1, 2, 3, 4, 5 };
	//! std::vector< int > v2{ 1, 2, 3, 4, 5.6 };

	print({ 1, 2, 3, 4, 5 });
	print({ "one", "two", "three", "four", "five" });

	return 0;
}

