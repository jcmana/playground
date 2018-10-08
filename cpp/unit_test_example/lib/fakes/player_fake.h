#pragma once

#include <string>


class IPlayer
{
public:
	struct Exceptions_Base:
		std::exception
	{
		Exceptions_Base(const char* what) :
			std::exception(what)
		{}
	};

	struct GuitarIsNotTurnedOn:
		Exceptions_Base
	{
		GuitarIsNotTurnedOn() :
			Exceptions_Base("Guitar is not turned on")
		{}
	};

	struct UnknownChord:
		Exceptions_Base
	{
		UnknownChord(const std::string& chord):
			Exceptions_Base((std::string("Unknown chord given - ") + chord).c_str())
		{}
	};

public:
	virtual void TurnGuitarOn() = 0;
	virtual std::string PlayChord(const std::string& chord) = 0;
	virtual void TurnGuitarOff() = 0;
};