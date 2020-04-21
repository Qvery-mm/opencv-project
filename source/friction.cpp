#include "head.h"


bool flag = true;
double _start_time, _time;
const double g = 9.81;

double answer()
{
	double x = points[1].first - points[0].first, y = points[1].second - points[0].second;
	return ( x / y - (x * x + y * y) / ( _time * g * x) );
}


int friction()
{
	if(points.size() < 2)
	{
		cout << "too few points";
		state = "run";
		return -1;
	}
	if(flag)
	{
		flag = false;
		printf("\n press Enter when you'll ready\n");
		waitKey(0);
		_start_time = clock();
	}
	if(center_now.first > points[1].first)
	{
		_time = clock() - _start_time;
		cout << answer();
		state = "run";
		flag = true;
	}


	return 0;
}
