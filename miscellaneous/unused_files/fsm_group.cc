//#include "../../inc/Scheduler/FSMGroup.h"
#include "FSMGroup.h"



FSMGroup::FSMGroup(std::shared_ptr<Scheduleable> mainState, std::pair<std::function<bool()>, std::shared_ptr<Scheduleable>> subStatesPack...)
	:RescheduleableGroup(mainState->GetRequirementFlags(), GroupTypes::FSM), mainState{ mainState }, subStates{ std::vector<std::shared_ptr<Scheduleable>>() }
{
	mainUnpackedID = Schedule(mainState);
	std::tuple< std::pair<std::function<bool()>, std::shared_ptr<Scheduleable>>> subStatesTuple = std::tuple< std::pair<std::function<bool()>, std::shared_ptr<Scheduleable>>>(subStatesPack);
	//Loop through subStatesTuple, adding scheduleables to subStates and setting the proper EndBehaviors
	//Also, PANICCCCCCCCCCCCCCCCCCCCCC
	//I don't know how I'm going to make subStates properly return to the main state, because I can't tag them with unique ScheduleableIDs
	//Actually, it's fine, since I can use their scheduleableIDs lol
	//Ok but there's another problem
	//I need to add functions that run when a scheduleable returns
}

FSMGroup::FSMGroup(std::shared_ptr<Scheduleable> mainState, std::function<bool()> shouldSwitch, std::shared_ptr<Scheduleable> subState)
	:FSMGroup(mainState, std::pair<std::function<bool()>, std::shared_ptr<Scheduleable>>(shouldSwitch, subState)){}

FSMGroup::FSMGroup(const FSMGroup& copyFSMGroup)
	:RescheduleableGroup(copyFSMGroup), mainUnpackedID{mainUnpackedID}, mainState {copyFSMGroup.mainState}, subStates{ copyFSMGroup.subStates }
{
}

void FSMGroup::Initialize()
{
	database.scheduleableMap[mainUnpackedID] = mainState;
	for (std::shared_ptr<Scheduleable> scheduleable : subStates)
	{
		scheduleable->Initialize();
		scheduleable->ResetAvailability();
	}
	RescheduleableGroup::Initialize();
}