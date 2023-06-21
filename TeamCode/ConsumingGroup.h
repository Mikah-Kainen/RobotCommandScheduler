#pragma once
#include "GroupBase.h"

class ConsumingGroup : public GroupBase
{
private:
	virtual void PreRun() override;

	virtual bool PostRun(std::vector<unsigned int> packedIDsToDelete) override;

protected:
	ConsumingGroup(unsigned char systemFlags, SchedulerTypes type);

	ConsumingGroup(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	ConsumingGroup(std::vector<Systems> schedulerSystems, SchedulerTypes type);

public:
	ConsumingGroup(const ConsumingGroup& copyConsumingGroup);
};

