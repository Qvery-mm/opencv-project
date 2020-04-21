#include "head.h"
#define f first
#define s second


const double pi = 3.141592654;
int counter = -2;
double string_length;
const int N_exp = 30;
double now;
double start_time;
bool a = true;

/**
 * points[0] is initial position of pendulum;
 * center_now is current position of pendulum;
 */
int pendulum()
{
	if(points.empty())
	{
		cout << "too few points";
		state = "run";
		return -1;
	}
	if(counter == -2)
	{
		cout << "enter string length: ";
		cin >> string_length;
		counter++;
	}
	if( (((center_now.f - points[0].f) > 0) && now < 0) || (  ((center_now.f - points[0].f) < 0) && now > 0)  )  
	{
		if(a)
		{
			start_time = clock();
			a = false; 
		}
		cout << '!';
		counter++;
	}
	now = center_now.f - points[0].f;
	if(counter == 2 * N_exp)
	{
		double time = (clock() - start_time) /1000;
		cout <<"time "<< time << endl;
		//double period = time/N_exp;
		double g = (4 * pi * pi * string_length * N_exp * N_exp) / (time * time);
		cout << endl <<"g = "<<g;
		state = "run";
		a = true;
		counter = -2;
	}

	return 0;
}