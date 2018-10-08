#pragma once

#include "raii_intf.h"

#include <memory>
#include <string>
#include <iostream>

class raii_impl : public raii_intf
{
public:
	class factory
	{
	public:
		virtual std::shared_ptr<raii_intf> create_raii() const;
	};

	raii_impl();
	~raii_impl();


// raii_intf interface implementation
public:
	virtual void execute() override;


protected:
	int * m_pointer;
};
