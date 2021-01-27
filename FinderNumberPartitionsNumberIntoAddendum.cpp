#include "FinderNumberPartitionsNumberIntoAddendum.h"

#include <stdexcept>

#include "ExecutionPolicy.h"


FinderNumberPartitionsNumberIntoAddendum::FinderNumberPartitionsNumberIntoAddendum()
{

}

FinderNumberPartitionsNumberIntoAddendum::ResultOperation FinderNumberPartitionsNumberIntoAddendum::setNumberOptions(uint32_t numPos, uint32_t maxValue)
{
	if (numPos == 0 || maxValue == 0)
	{
		return ResultOperation::FAILED;
	}

	if (numPos > numPos_)
	{
		factorial_.resize(numPos + 1);
		factorial_[0] = 1;
		for (uint64_t i = numPos_ + 1; i <= numPos; i++)
		{
			factorial_[i] = factorial_[i - 1] * i;
		}
	}

	numPos_ = numPos;
	maxValue_ = maxValue;
	valuesCount_.clear();

	return ResultOperation::SUCCESS;
}

std::vector<double> FinderNumberPartitionsNumberIntoAddendum::getBeautifulNumberCount() const
{
	if (numPos_ == 0 || maxValue_ == 0)
	{
		return valuesCount_;
	}

	if (valuesCount_.size() == 0)
	{
		calcBeautifulNumberCount();
	}

	return generateFullVecValues();
}

std::vector<double> FinderNumberPartitionsNumberIntoAddendum::generateFullVecValues() const
{
	uint64_t n = numPos_ * maxValue_ + 1;
	uint64_t halfN = valuesCount_.size();
	std::vector<double> values(n, 0);

	std::transform(TRY_PARALLELIZE_PAR_UNSEQ valuesCount_.cbegin(), valuesCount_.cbegin() + halfN, values.begin(), [this](const uint64_t& value) {return value; });

	for (uint64_t i = halfN; i < n; i++)
	{
		values[i] = valuesCount_[n - i - 1];
	}
	
	return values;
}

void FinderNumberPartitionsNumberIntoAddendum::calcBeautifulNumberCount() const
{
	uint64_t n = numPos_ * maxValue_ + 1;

	uint64_t num = static_cast<uint64_t>(ceil(static_cast<double>(n) / 2.0));

	if (numPos_ == 1)
	{
		valuesCount_ = std::vector<double>(num, 1);
		return;
	}

	valuesCount_.resize(num);

	std::vector<uint64_t> values(num, 0);
	for (uint64_t i = 0; i < num; i++)
	{
		values[i] = i;
	}

	std::transform(TRY_PARALLELIZE_PAR_UNSEQ values.cbegin(), values.cend(), valuesCount_.begin(), [&](uint64_t value) {return calcDuplicateValues(value); });
	
	return;
}

double FinderNumberPartitionsNumberIntoAddendum::factorialTable(uint32_t value) const
{
	return factorial_[value];
}

std::map<uint64_t, uint64_t> FinderNumberPartitionsNumberIntoAddendum::calcNumberOfRepetitions(const std::vector<uint64_t>& vec) const
{
	std::vector<uint64_t> vecRepeat;
	std::map<uint64_t, uint64_t> mapPair;
	for (auto i : vec)
	{
		auto it = mapPair.find(i);
		if (it != std::cend(mapPair))
		{
			(*it).second++;
		}
		else
		{
			mapPair.emplace(std::pair<uint64_t, uint64_t>(i, 1));
		}
	}
	return mapPair;
}

double FinderNumberPartitionsNumberIntoAddendum::permutationWithRepetitions(const std::map<uint64_t, uint64_t>& vec) const
{
	double n = factorialTable(numPos_);

	if (n == 0)
	{
		throw std::invalid_argument("");
	}

	double m = 1;
	for (const auto& i : vec)
	{
		m *= factorialTable(i.second);
	}

	if (m == 0)
	{
		throw std::invalid_argument("");
	}

	double res = n / m;
	return res;
}

double FinderNumberPartitionsNumberIntoAddendum::calcDuplicateValues(uint64_t value) const
{
	if (value == 0)
	{
		return 1;
	}

	uint64_t currValue = value;
	uint64_t currMaxValue = maxValue_;
	if (value < maxValue_)
	{
		currMaxValue = value;
	}
	uint64_t curPos = 0;
	double res = 0;

	std::vector<uint64_t> vec(numPos_, 0);

	for (uint64_t j = 0; j < maxValue_; j++)
	{
		if (currMaxValue * numPos_ < value)
		{
			return res;
		}

		for (uint64_t i = 0; i < numPos_; i++)
		{
			if (currValue > currMaxValue)
			{
				vec[i] = currMaxValue;
				currValue -= currMaxValue;
			}
			else
			{
				vec[i] = currValue;
				currValue = 0;
				curPos = i;
				i = numPos_;
			}
		}

		if (curPos != 0 && curPos == numPos_ - 1)
		{
			curPos--;
		}

		res += scatter(vec, curPos);
		
		if (currMaxValue == 1)
		{
			return res;
		}

		currValue = value;
		currMaxValue--;
	}

	return res;
}

double FinderNumberPartitionsNumberIntoAddendum::scatter(std::vector<uint64_t> vec, uint64_t pos) const
{
	double res = 0;
	res += permutationWithRepetitions(calcNumberOfRepetitions(vec));

	if (pos == 0)
	{
		return res;
	}

	bool flag = true;

	while (flag)
	{
		uint64_t n = vec.size();
		uint64_t num = vec.size() - 1 - pos;
		for (uint64_t i = pos + 1; i < n; i++)
		{
			if (vec[pos] - 1 > vec[pos + 1])
			{
				vec[i]++;
				vec[pos]--;

				if (pos > 1)
				{
					if (vec[pos - 1] > vec[pos] + 1)
					{
						res += scatter(vec, pos - 1);
					}
					else
					{
						res += permutationWithRepetitions(calcNumberOfRepetitions(vec));
					}
				}
				else
				{
					res += permutationWithRepetitions(calcNumberOfRepetitions(vec));
				}
			}
			else
			{
				flag = false;
			}
		}
	}

	return res;
}