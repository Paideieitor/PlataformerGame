#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:

	Timer();
	~Timer();

	void Start();

	int Read();
	float ReadSec();

	int start;
};

#endif