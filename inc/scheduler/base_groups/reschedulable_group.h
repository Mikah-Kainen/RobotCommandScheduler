#pragma once

#include "group_base.h"

class ReschedulableGroup : public GroupBase
{
private:
	//bool shouldInitializeOrHasRestarted = true;
	//void InitializeGroup() override;
	std::unordered_set<unsigned int> initializeRequirementFreeSchedulables;

protected:
	virtual void Remove(unsigned int packedID) override;

	virtual bool Return(bool isFinished) override;

	//virtual void InitializeBody();

	ReschedulableGroup(unsigned char systemFlags, GroupTypes type);

	ReschedulableGroup(std::vector<unsigned char> systemFlags, GroupTypes type);

	ReschedulableGroup(std::vector<Systems> schedulerSystems, GroupTypes type);

public:
	ReschedulableGroup(const ReschedulableGroup& copyReschedulableGroup);

	void Initialize() override;
};

