#pragma once
#include <functional>
#include <vector>
#include <unordered_map>

/*
	Each SharedState update function should only update one variabele.
	For example, a robot shared state might have four sensor variable states.
	Each update would update one sensor variable state.
	Within the sensor variable state, multiple values might update, but that is an unrelated issue.
	Programming with this policy helps enforce the best practice of having functions do one thing.


	*** Important Note
	Storing references is disabled.
	This is because the nature of references does not align with the goal of shared state.
	You can't update the reference to something else after, just change the value of the original thing you were referencing.
	In the shared state, you should have actual values that you update or get references too.
	If you want a reference, put it in a substate.
	For our sensor example, if sensors need data about the position, the sensor state itself can store a const reference to the position, which would then be accessible in the update function.
*/

template <typename... Ts>
class SharedState
{
	using IndexType = unsigned char;

public:
	const int StoreCount = sizeof...(Ts);

public:
	std::tuple<Ts...> store;

	std::vector<std::function<void()>>* updateFuncs;

public:

	SharedState(Ts... values)
		: store{ std::tuple<Ts...>(values...) }, updateFuncs{ new std::vector<std::function<void()>>[StoreCount] }
	{
		for (IndexType i = 0; i < StoreCount; i++) {
			updateFuncs[i] = std::vector<std::function<void()>>();
		}
	}

	~SharedState() {
		delete[] updateFuncs;
	}

	//Will throw compile-time error if indices are out of range
	template <IndexType... indices>
	std::tuple<std::tuple_element_t<indices, std::tuple<Ts...>>...> Extract() {
		return std::tuple<std::tuple_element_t<indices, std::tuple<Ts...>>...>(std::get<indices>(store)...);
	}

	//Will throw compile-time error if indices are out of range
	template <typename FuncType, IndexType... indices>
	auto Apply(std::function<FuncType> func) {
		return func(std::get<indices>(store)...);
	}

	//Base case for set
	//Will throw compile-time error if indices are out of range
	template <IndexType index>
	void Set(std::tuple_element_t<index, std::tuple<Ts...>> value) {
		std::get<index>(store) = value;
	}

	//Recursive case for set
	//Will throw compile-time error if indices are out of range
	template <IndexType index1, IndexType index2, IndexType... indices>
	void Set(std::tuple_element_t<index1, std::tuple<Ts...>> value1, std::tuple_element_t<index2, std::tuple<Ts...>> value2, std::tuple_element_t<indices, std::tuple<Ts...>>... values) {
		std::get<index1>(store) = value1;
		Set<index2, indices...>(value2, values...);
	}

	//Updates via reference to the target value
	//Will throw compile-time error if indices are out of range
	//Interesting Stack Overflow post about passing lambdas around: https://stackoverflow.com/questions/7941562/what-is-the-lifetime-of-a-c-lambda-expression
	template <IndexType index>
	void AddUpdate(std::function<void(std::tuple_element_t<index, std::tuple<Ts...>>&)> updateFunc) {
		updateFuncs[index].push_back([=]() { Apply<void(std::tuple_element_t<index, std::tuple<Ts...>>&), index>(updateFunc); return; }); //I store updateFunc by value since it might be a lambda created only as an argument for the function call
	}

	//Updates by setting the target value to the result of the function
	//Will throw compile-time error if indices are out of range
	template <IndexType index>
	void AddUpdateV(std::function<std::tuple_element_t<index, std::tuple<Ts...>>(std::tuple_element_t<index, std::tuple<Ts...>>)> updateFunc) {
		updateFuncs[index].push_back([=]() { Set<index>(Apply<std::tuple_element_t<index, std::tuple<Ts...>>(std::tuple_element_t<index, std::tuple<Ts...>>), index>(updateFunc)); return; });
	}


	void Update() {
		for (IndexType i = 0; i < StoreCount; i++) {
			for (std::function<void()> updateFunc : updateFuncs[i]) {
				updateFunc();
			}
		}
	}

	//Will throw compile-time error if indices are out of range
	//Empty case will match with other Update
	template <IndexType... indices>
	void Update() {
		for (IndexType i : {indices...}) {
			for (std::function<void()> updateFunc : updateFuncs[i]) {
				updateFunc();
			}
		}
	}
};