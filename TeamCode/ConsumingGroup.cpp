//#include "../../inc/SchedulerInc/ConsumingGroup.h"
#include "ConsumingGroup.h"



//void ConsumingGroup::InitializeGroup()
//{
//	//ConsumingGroups should initialize in their constructors
//}

void ConsumingGroup::Remove(unsigned int packedID)
{
	database.Remove(Unpack(packedID));
}

bool ConsumingGroup::Return(bool isFinished)
{
	return isFinished;
}

ConsumingGroup::ConsumingGroup(unsigned char systemFlags, GroupTypes type)
	:GroupBase(systemFlags, type)
{

}

ConsumingGroup::ConsumingGroup(std::vector<unsigned char> systemFlags, GroupTypes type)
	:GroupBase(systemFlags, type)
{

}

ConsumingGroup::ConsumingGroup(std::vector<Systems> schedulerSystems, GroupTypes type)
	:GroupBase(schedulerSystems, type)
{

}

ConsumingGroup::ConsumingGroup(const ConsumingGroup& copyConsumingGroup)
	:GroupBase(copyConsumingGroup)
{

}