#pragma once
#include "SchedulerBase.h"

class Scheduler : public SchedulerBase
{
private:
	Scheduler();

public:
	static Scheduler& GetInstance();

	void Schedule(Scheduleable* scheduleable);

	void Schedule(std::function<bool()> function, unsigned char requirementFlags);

	void Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);
};