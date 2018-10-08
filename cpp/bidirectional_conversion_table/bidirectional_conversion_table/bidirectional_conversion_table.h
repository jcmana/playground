#pragma once

#include <vector>
#include <map>
#include <utility>
#include <type_traits>

template<typename A, typename B>
class bidirectional_conversion_table
{
public:
	explicit bidirectional_conversion_table(const std::vector<std::pair<A, B>> && t);
	A convert(B b) const;
	B convert(A a) const;

	A operator[](B b) const;
	B operator[](A a) const;

private:
	std::vector<std::pair<A, B>> m_storage;
	std::map<A, typename std::vector<std::pair<A, B>>::const_iterator> m_AtoB;
	std::map<B, typename std::vector<std::pair<A, B>>::const_iterator> m_BtoA;
};

template<typename A, typename B>
bidirectional_conversion_table<A, B>::bidirectional_conversion_table(const std::vector<std::pair<A, B>> && t)
{
	static_assert((std::is_same<A, B>::value == false), "Template types must differ.");

	m_storage = std::move(t);

	for (std::vector<std::pair<A, B>>::const_iterator i = m_storage.cbegin(); i < m_storage.cend(); ++i)
	{
		m_AtoB[i->first] = i;
		m_BtoA[i->second] = i;
	}
}

template<typename A, typename B>
A bidirectional_conversion_table<A, B>::convert(B b) const
{
	return m_BtoA.at(b)->first;
}

template<typename A, typename B>
B bidirectional_conversion_table<A, B>::convert(A a) const
{
	return m_AtoB.at(a)->second;
}

template<typename A, typename B>
A bidirectional_conversion_table<A, B>::operator[](B b) const
{
	return convert(b);
}

template<typename A, typename B>
B bidirectional_conversion_table<A, B>::operator[](A a) const
{
	return convert(a);
}
