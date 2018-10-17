#pragma once

#include <string>

class node
{
public:
	node(std::string name = "node") :
		m_name(name)
	{
	}

	std::string name() const
	{
		return m_name;
	}

private:
	std::string m_name;
};