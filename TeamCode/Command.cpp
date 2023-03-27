#include <functional>
#include <exception>

//class IScheduleable
//{
//private:
//
//public:
//	
//	IScheduleable()
//	{
//	}
//
//	char RequirementFlags;
//	char AvailableSystems; //used by scheduler to track if all the required systems are available
//};

static enum class Systems
{
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Other, //Other should always be last because it is used to track the length of Systems
};

//Link about Variadic Templates: https://en.cppreference.com/w/cpp/language/parameter_pack
template <typename... Ts>
class Command
{
private:
	std::function<bool(Ts...)> backingFunction;
	char requirementFlags;

public:

	Command(std::function<bool(Ts...)> backingFunction)
		: backingFunction{ backingFunction } {}


	//Use for testing purposes if needed
	//bool Activate(Ts... params)
	//{
	//	return backingFunction(params...);
	//}

	void AddRequirement(Systems requirement)
	{
		requirementFlags |= requirement;
	}

	void Schedule(Ts... params)
	{
		throw std::exception("Not implemented");
		//have a function in scheduler that takes in a backing function and requirements and makes a scheduledFunction
		//^also it adds the scheduledFunction to the proper deques
	}
};
