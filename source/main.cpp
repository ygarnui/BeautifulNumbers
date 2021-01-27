#include "FinderNumberBeautifulNumbers.h"

#include <ctime>
#include <iostream>

int main()
{
	FinderNumberBeautifulNumbers temp;
	
	size_t startTime = 0;
	size_t endTime = 0;
	size_t searchTime = 0;

	std::cout << "start computation" << "\n";
	startTime = clock();

	uint64_t res = temp.getNumberBeautifulNumbers();

	endTime = clock();
	searchTime = endTime - startTime;

	std::cout << "computation time: " << searchTime << "\n";

	std::cout << "count num = " << res;
}

