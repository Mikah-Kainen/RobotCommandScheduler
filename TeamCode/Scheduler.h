#pragma once
#include "SchedulerBase.h"

class Scheduler : public SchedulerBase
{
private:
	Scheduler();

public:
	static Scheduler& GetInstance();
};