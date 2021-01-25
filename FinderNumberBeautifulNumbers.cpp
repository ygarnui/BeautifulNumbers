#include "FinderNumberBeautifulNumbers.h"

#include <algorithm>
#include <numeric>

#include "ExecutionPolicy.h"

FinderNumberBeautifulNumbers::FinderNumberBeautifulNumbers()
{
	
}

uint64_t FinderNumberBeautifulNumbers::getNumberBeautifulNumbers()
{
	uint64_t maxValue = 12;
	uint64_t pos = 6;
	//285793549106

	if (finder_.setNumberOptions(pos, maxValue) == FinderNumberPartitionsNumberIntoAddendum::ResultOperation::FAILED)
	{
		return 0;
	}

	auto values = finder_.getBeautifulNumberCount();

	std::transform(TRY_PARALLELIZE_PAR_UNSEQ values.cbegin(), values.cend(), values.begin(), [](uint64_t value) {return value * value; });

	double res = std::reduce(TRY_PARALLELIZE_PAR_UNSEQ values.cbegin(), values.cend(), double(), std::plus<>());

	res = res * (maxValue + 1);

	return res;
}
