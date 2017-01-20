#pragma once

#include <set>
#include <list>
#include <map>

namespace sfext
{
	template <class T, class U>
	std::set<T>  getFirstValues (const std::map<T, U> & map)
	{
		// Returns a set of keys for the given map
		std::set<T> result;
		for (const auto & pair : map)
			result.insert(pair.first);
		return result;
	}

	template <class T, class U>
	std::list<U> getSecondValues(const std::map<T, U> & map)
	{
		// Returns the value associate with each key of the given map
		std::list<U> result;
		for (const auto & pair : map)
			result.insert(pair.second);
		return result;
	}
}