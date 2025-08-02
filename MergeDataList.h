#pragma once
#include<vector>

struct DataList
{
	std::vector<double>time;
	std::vector<std::vector<double>>dataValues;
};


class MergeDataList
{
public:
	DataList list1;
	DataList list2;
public:
	MergeDataList();

	~MergeDataList();
private:
	


};

