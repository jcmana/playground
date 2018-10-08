#pragma once

#include <string>
#include <exception>

class IGuitar
{
public:
	// List of exceptions which can be thrown from IGuitar interface:

	// Base exception, each thrown exception is derived from this.
	struct Exceptions_Base:
		std::exception
	{
		Exceptions_Base(const char* what):
			std::exception(what)
		{}
	};

	struct TearString:
		Exceptions_Base
	{
		TearString(int s):
			Exceptions_Base((std::string("Tear string no. ") + std::to_string(s)).c_str())
		{}
	};

public:
	/*
		Play chord on guitar.
		params:
			params is a frett position on each string. Value -1 means string is not used in chord.

		returns:
			sound of chord given by fretts.

		exceptions:
			TearString if string is during play teared.
	*/
	virtual std::string PlayChord(int sE, int sA, int sD, int sG, int sH, int se) = 0;
};