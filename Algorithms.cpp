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

std::vector<double> computeTiltAngleToEarth(const std::vector<double>& accX, const std::vector<double>& accY)
{
	size_t size = accX.size();
	std::vector<double> angles(size);

#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < size; ++i)
	{
		// Angle in radians between the vector (accX, accY) and the vertical axis (0, -1)
		angles[i] = std::atan2(accX[i], -accY[i]); // atan2(y, x): handles all quadrants
		//angles[i] *= RAD_TO_DEG; // uncomment if you want degrees    use instead: const double pi = 3.1415926535897932385;
	}

	return angles;
}