#include "Algorithms.h"

size_t findNextLowerElementIndex(const std::vector<double>& sortedList, const size_t listSize, const double target)
{
	size_t l = 0, r = listSize;
	while (l < r) {
		size_t m = (l + r) >> 1;
		(sortedList[m] <= target ? l : r) = m + (sortedList[m] <= target);
	}
	return l ? l - 1 : 0;
}
