#pragma once
#include "RescheduleableGroup.h"

class ConditionalGroup : RescheduleableGroup
	//Rename to condition and get rid of constructor taking in a vector of Scheduleables
{
private:
	std::function<bool()> condition;

protected:

public:
	//ConditionalGroups are very useful as wrappers for other Groups, this is syntax supporting that
	ConditionalGroup(std::shared_ptr<Scheduleable> scheduleableToSchedule, std::function<bool()> condition);

	ConditionalGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool()> condition);

	ConditionalGroup(const ConditionalGroup& copyConditionalGroup);

	void Initialize() override;
};

