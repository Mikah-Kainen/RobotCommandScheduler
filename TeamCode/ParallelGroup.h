#pragma once
#include "GroupBase.h"

class ParallelGroup : public GroupBase
{
private:


protected:


public:

	//ParallelGroup(std::vector<Scheduleable> scheduleableList, unsigned char requirements);

	ParallelGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule);
};
