#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:

	Timer(int offset = 0);
	~Timer();

	void Start();

	int Read();
	float ReadSec();

	void Pause();
	void Play();

private:

	int start;
	int pausedAt;
};

#endif