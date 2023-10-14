#pragma once
#include "GroupBase.h"

class RescheduleableGroup : public GroupBase
{
private:
	//bool shouldInitializeOrHasRestarted = true;
	//void InitializeGroup() override;
	std::unordered_set<unsigned int> initializeRequirementFreeScheduleables;

protected:
	virtual void Remove(unsigned int packedID) override;

	virtual bool Return(bool isFinished) override;

	//virtual void InitializeBody();

	RescheduleableGroup(unsigned char systemFlags, GroupTypes type);

	RescheduleableGroup(std::vector<unsigned char> systemFlags, GroupTypes type);

	RescheduleableGroup(std::vector<Systems> schedulerSystems, GroupTypes type);

public:
	RescheduleableGroup(const RescheduleableGroup& copyRescheduleableGroup);

	void Initialize() override;
};

