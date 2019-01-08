#pragma once

#include <map>
#include <utility>

template<typename Key, typename T, T value = T()>
class defaultable_map :
	public std::map<Key, T>
{
public:
	// inherit std::unordered_map constructors
	using std::map<Key, T>::map;

	T & operator[](const Key & key)
	{
		auto it = find(key);

		if (it == end())
		{
			// insert default value
			auto result = insert(std::make_pair(key, value));
			it = result.first;
		}

		return it->second;
	}
};
