#pragma once
#include "GroupBase.h"

class RescheduleableGroup : public GroupBase
{
private:
	virtual void PreRun() override;

	virtual bool PostRun(std::vector<unsigned int> packedIDsToDelete) override;

protected:
	RescheduleableGroup(unsigned char systemFlags, SchedulerTypes type);

	RescheduleableGroup(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	RescheduleableGroup(std::vector<Systems> schedulerSystems, SchedulerTypes type);

public:
	RescheduleableGroup(const RescheduleableGroup& copyRescheduleableGroup);
};

