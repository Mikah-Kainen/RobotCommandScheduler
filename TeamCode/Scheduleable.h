#pragma once
#include <functional>
#include "Static.h"

class Scheduleable
	{
	private:
		unsigned char availableSystems;
		std::function<bool()> backingFunction; //Change to virtual function

		virtual bool Run();

	protected:
		Scheduleable(unsigned char requirementFlags);

	public:
		//virtual void ThisIsAbstract() = 0;

		bool IsDead;
		unsigned char requirementFlags;

		Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags);

		Scheduleable(std::function<bool()> backingFunction, Systems requiredSystem);

		//Scheduleable(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy);

		//Scheduleable(std::function<bool()> backingFunctionCopy, Systems requiredSystem, bool passingByCopy);

		//Scheduleable();

		Scheduleable(const Scheduleable& copyScheduelable);

		~Scheduleable();

		unsigned char GetRequirementFlags();

		void AddRequirement(unsigned char newRequirementFlags);

		bool RunIfReady(unsigned char availableSystem);

		void ResetAvailability();
	};