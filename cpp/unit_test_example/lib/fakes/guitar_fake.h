#pragma once

#include <string>

struct CGuitar_fake:
	public IGuitar
{
	virtual std::string PlayChord(int sE, int sA, int sD, int sG, int sH, int se) override
	{
		// Save input params:
		m_sE = sE;
		m_sA = sA;
		m_sD = sD;
		m_sG = sG;
		m_sH = sH;
		m_se = se;
		m_wasCalled = true;

		// Simulation of TearString exception is requested:
		if (m_tearString) {
			throw IGuitar::TearString(m_tearString);
		}

		// Pass fake output:
		return m_rv;
	}

	bool m_wasCalled = false;
	int m_tearString = 0;
	int m_sE = -2;
	int m_sA = -2;
	int m_sD = -2;
	int m_sG = -2;
	int m_sH = -2;
	int m_se = -2;
	std::string m_rv;
};