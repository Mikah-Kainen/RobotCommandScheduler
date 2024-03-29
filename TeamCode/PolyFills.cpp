#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <list>

class PolyFills
{
private:
	PolyFills(){}

public:
	static PolyFills& GetInstance()
	{
		static PolyFills polyFills = PolyFills();
		return polyFills;
	}

#pragma region c++17Polyfills
private:
	template <std::size_t... Indices>
	struct indices {};

	template <std::size_t N, std::size_t... Is>
	struct build_indices
		: build_indices<N - 1, N - 1, Is...> {};

	template <std::size_t... Is>
	struct build_indices<0, Is...> : indices<Is...> {};

	template <typename Tuple>
	using IndicesFor = build_indices<std::tuple_size<Tuple>::value>;


	template <typename FuncT, typename Tuple, std::size_t... Indices>
	bool Apply(FuncT& func, Tuple params, indices<Indices...>) //Maybe look into std::forward and params being Tuple&&
	{
		return  func(std::get<Indices>(params)...);
	}

public:
	template <typename FuncT, typename Tuple>
	bool Apply(FuncT& func, Tuple params) //Maybe look into std::forward and params being Tuple&&
	{
		return Apply(func, params, IndicesFor<Tuple>{});
	}
#pragma endregion
};