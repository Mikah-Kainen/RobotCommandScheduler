#pragma once
#include "base_groups/consuming_group.h"

class Scheduler : public ConsumingGroup
{
private:
	Scheduler();

public:
	static Scheduler& GetInstance();

	void Schedule(std::shared_ptr<Schedulable> schedulable);

	void Schedule(std::function<bool()> function, unsigned char requirementFlags); //look here

	void Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);
};