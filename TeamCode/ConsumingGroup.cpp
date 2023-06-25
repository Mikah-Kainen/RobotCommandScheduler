//#include "../../inc/SchedulerInc/ConsumingGroup.h"
#include "ConsumingGroup.h"



void ConsumingGroup::Initialize()
{
	//ConsumingGroups should initialize in their constructors
}

void ConsumingGroup::Remove(unsigned int packedID)
{
	database.Remove(Unpack(packedID));
}

bool ConsumingGroup::Return(bool isFinished)
{
	return isFinished;
}

ConsumingGroup::ConsumingGroup(unsigned char systemFlags, SchedulerTypes type)
	:GroupBase(systemFlags, type)
{

}

ConsumingGroup::ConsumingGroup(std::vector<unsigned char> systemFlags, SchedulerTypes type)
	:GroupBase(systemFlags, type)
{

}

ConsumingGroup::ConsumingGroup(std::vector<Systems> schedulerSystems, SchedulerTypes type)
	:GroupBase(schedulerSystems, type)
{

}

ConsumingGroup::ConsumingGroup(const ConsumingGroup& copyConsumingGroup)
	:GroupBase(copyConsumingGroup)
{

}