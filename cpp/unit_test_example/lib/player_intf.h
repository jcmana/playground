#pragma once

#include <string>


class IPlayer
{
public:
	// List of exceptions which can be thrown from IPlayer interface:

	// Base exception, each thrown exception is derived from this.
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
	/*
		Run guitar.
		exceptions:
		 - std::exception if something terrible failed.
	*/
	virtual void TurnGuitarOn() = 0;

	/*
		Play chord.
		params:
		 - chord Chord to play, currently supported [Dmi, H7].
		return:
		 - Sound of chord in format "$chord sounds like '$sound'".
		exceptions:
		 - GuitarIsNotTurnedOn If TurnGuitarOn wasnt called.
		 - UnknownChord If unknown chord is provided.
	*/
	virtual std::string PlayChord(const std::string& chord) = 0;

	/*
		Turn guitar off.
	*/
	virtual void TurnGuitarOff() noexcept = 0;
};