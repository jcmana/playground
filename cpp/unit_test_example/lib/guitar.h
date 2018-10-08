#pragma once

#include "guitar_intf.h"
#include <memory>


class CGuitar:
	public IGuitar
{
public:
	// Factory for dependency injection:
	struct Factory
	{
		// !!! This must be virtual method, which create impl and return pointer to Interface.
		virtual std::shared_ptr<IGuitar> CreateGuitar() const
		{
			return std::make_shared<CGuitar>();
		}
	};

public:
	virtual std::string PlayChord(int sE, int sA, int sD, int sG, int sH, int se) override;
};