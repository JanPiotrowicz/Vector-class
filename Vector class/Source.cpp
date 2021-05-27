#include <iostream>
#include "Vector.h"
#include "testStruct.h"

using namespace std;

int main()
{
	{
		Vector<int> vec;
		vec.push_many(1, 2, 3, 4, 5);

		vec[2] = 8;
		vec[1] = 7;
		vec[0] = 6;

		vec.print();
	}

	cin.get();
	return 0;
}
