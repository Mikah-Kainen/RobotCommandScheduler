#pragma once
#include "GroupBase.h"

class RescheduleableGroup : public GroupBase
{
private:
	//bool shouldInitializeOrHasRestarted = true;
	//void InitializeGroup() override;

protected:
	virtual void Remove(unsigned int packedID) override;

	virtual bool Return(bool isFinished) override;

	//virtual void InitializeBody();

	RescheduleableGroup(unsigned char systemFlags, SchedulerTypes type);

	RescheduleableGroup(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	RescheduleableGroup(std::vector<Systems> schedulerSystems, SchedulerTypes type);

public:
	RescheduleableGroup(const RescheduleableGroup& copyRescheduleableGroup);

	bool Initialize() override;
};

