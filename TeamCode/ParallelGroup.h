#pragma once
#include "SchedulerBase.h"

class ParallelGroup : public SchedulerBase
{
private:


protected:


public:

	//ParallelGroup(std::vector<FunctionManager::Scheduleable> scheduleableList, unsigned char requirements);

	ParallelGroup(std::vector<FunctionManager::Scheduleable*> scheduleablesToSchedule);
};