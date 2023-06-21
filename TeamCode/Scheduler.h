#pragma once
#include "RescheduleableGroup.h"

class Scheduler : public RescheduleableGroup
{
private:
	Scheduler();

public:
	static Scheduler& GetInstance();

	void Schedule(std::shared_ptr<Scheduleable> scheduleable);

	void Schedule(std::function<bool()> function, unsigned char requirementFlags); //look here

	void Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);
};