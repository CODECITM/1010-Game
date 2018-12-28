#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer(bool run = true);

	void Start();
	float Stop();
	void Reset();

	uint32 Read() const;
	float ReadSec() const;

	bool IsRunning();

private:
	bool running;

	uint32	started_at;
	uint32	stopped_at;
};

#endif //__j1TIMER_H__