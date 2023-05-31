#pragma once
#include "SchedulerBase.h"

class Scheduler : public SchedulerBase
{
private:
	Scheduler();

public:
	static Scheduler& GetInstance();

	void Schedule(std::shared_ptr<Scheduleable> scheduleable);

	void Schedule(const std::function<bool()>& function, unsigned char requirementFlags); //look here

	void Schedule(const std::function<bool()>& function, std::vector<Systems> requiredSystems);
};