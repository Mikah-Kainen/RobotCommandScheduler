#pragma once
#include "GroupBase.h"

class ParallelGroup : public GroupBase
{
private:


protected:


public:

	//ParallelGroup(std::vector<FunctionManager::Scheduleable> scheduleableList, unsigned char requirements);

	ParallelGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule);
};
