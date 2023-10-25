#pragma once
#include "RescheduleableGroup.h"

class FSMGroup : public RescheduleableGroup
{
private:
	int mainUnpackedID;
	std::shared_ptr<Scheduleable> mainState;
	std::vector<std::shared_ptr<Scheduleable>> subStates;

protected:

public:
	FSMGroup(std::shared_ptr<Scheduleable> mainState, std::pair<std::function<bool()>, std::shared_ptr<Scheduleable>> subStatesPack...);

	FSMGroup(std::shared_ptr<Scheduleable> mainState, std::function<bool()> shouldSwitch, std::shared_ptr<Scheduleable> subState);

	FSMGroup(const FSMGroup& copyFSMGroup);

	void Initialize() override;
};


