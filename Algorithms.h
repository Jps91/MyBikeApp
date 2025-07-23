#pragma once
#include <vector>
#include <cmath>
#include "DataStructure.h"

QuanternionList importFromRadPerSecond();	//this is the Quanterniondelta
size_t findNextLowerElementIndex(const std::vector<double>& sortedList, const size_t listSize, const double target);
std::vector<double> computeTiltAngleToEarth(const std::vector<double>& accX, const std::vector<double>& accY);
