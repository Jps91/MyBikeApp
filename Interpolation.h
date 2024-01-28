#pragma once
class Interpolation
{
public:
	Interpolation();
	void linearSet(double x0, double y0, double x1, double y1);
	double linear(double x0, double y0, double x1, double y1, double x); //Eg. x0,1=time y0,1= vlaue x=where you want the corresponing y point from
	double linearSameIntervall(double x); //Eg. x0,1=time y0,1= vlaue x=where you want the corresponing y point from
	double linearNext(double nextX1, double nextY1, double x);
	double bilinear(double x0, double y0, double x1, double y1, double x);

private:
	double linearCacheX0 = 0;
	double linearCacheX1 = 0;
	double linearCacheY0 = 0;
	double linearCacheY1 = 0;
	
	double linearCahceDeltaX = 0;
	double linearCahceDeltaY = 0;
};

