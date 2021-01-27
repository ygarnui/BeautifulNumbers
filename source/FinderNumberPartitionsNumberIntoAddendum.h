#pragma once

#include <math.h>

#include <atomic>
#include <optional>

#include <vector>
#include <map>

class FinderNumberPartitionsNumberIntoAddendum
{
public:

	enum class ResultOperation
	{
		SUCCESS = 0,
		FAILED
	};

	FinderNumberPartitionsNumberIntoAddendum();

	ResultOperation setNumberOptions(uint32_t numPos, uint32_t maxValue);

	std::vector<double> getBeautifulNumberCount() const;

private:

	std::vector<double> generateFullVecValues() const;

	void calcBeautifulNumberCount() const;

	double factorialTable(uint32_t value) const;

	std::map<uint64_t, uint64_t> calcNumberOfRepetitions(const std::vector<uint64_t>& vec) const;
	double permutationWithRepetitions(const std::map<uint64_t, uint64_t>& vec) const;

	double calcDuplicateValues(uint64_t value) const;

	double scatter(std::vector<uint64_t> vec, uint64_t pos) const;

	uint64_t numPos_ = 0;
	uint64_t maxValue_ = 0;

	mutable std::vector<double> valuesCount_;

	std::vector<double> factorial_;
};

