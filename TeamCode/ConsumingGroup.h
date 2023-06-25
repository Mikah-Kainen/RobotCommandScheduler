#pragma once
#include "GroupBase.h"

class ConsumingGroup : public GroupBase
{
private:

protected:
	virtual void InitializeGroup() override;

	virtual void Remove(unsigned int packedID) override;

	virtual bool Return(bool isFinished) override;

	ConsumingGroup(unsigned char systemFlags, SchedulerTypes type);

	ConsumingGroup(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	ConsumingGroup(std::vector<Systems> schedulerSystems, SchedulerTypes type);

public:
	ConsumingGroup(const ConsumingGroup& copyConsumingGroup);
};

