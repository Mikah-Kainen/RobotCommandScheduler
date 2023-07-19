#pragma once
#include <functional>
#include <memory>
#include "Static.h"

class Scheduleable
	{
	private:
		enum class ScheduleableStates
		{
			Initialize,
			Run,
			Cleanup,
			Return,
		};

		static ScheduleableStates GetNextState(ScheduleableStates state);

		unsigned char availableSystems;
		std::function<bool()> backingFunction; //Change to virtual function
		std::shared_ptr<Scheduleable> initializationScheduleable;
		std::shared_ptr<Scheduleable> cleanupScheduleable;
		ScheduleableStates startingState;
		ScheduleableStates endingState;

		virtual bool Run();

	protected:
		Scheduleable(unsigned char requirementFlags);

		void SetInitializationScheduleableDirect(std::shared_ptr<Scheduleable> scheduleable);

		void SetCleanupScheduleableDirect(std::shared_ptr<Scheduleable> scheduleable);

	public:
		//virtual void ThisIsAbstract() = 0;
		ScheduleableStates currentState;
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

		virtual bool Initialize();

		bool SetInitializationScheduleable(std::shared_ptr<Scheduleable> scheduleable);

		bool SetCleanupScheduleable(std::shared_ptr<Scheduleable> scheduleable);
	};