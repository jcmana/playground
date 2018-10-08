#pragma once

#include "player_intf.h"

// You have to include implementation (not intf), because you have an access to creation of class.
#include "guitar.h"

#include <memory>


class CPlayer:
	public IPlayer,
	protected CGuitar::Factory // Injection of dependency on CGuitar class.
{
public:
	struct Factory
	{
		virtual std::shared_ptr<IPlayer> CreatePlayer() const
		{
			return std::make_shared<CPlayer>();
		}
	};

public:
	virtual void TurnGuitarOn() override;
	virtual std::string PlayChord(const std::string& chord) override;
	virtual void TurnGuitarOff() noexcept override;

private:
	std::shared_ptr<IGuitar> m_spGuitar; // Your guitar.
};