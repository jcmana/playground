#include "player.h"


void CPlayer::TurnGuitarOn()
{
	// Create guitar:
	m_spGuitar = CreateGuitar();
}


std::string CPlayer::PlayChord(const std::string& chord)
{
	// If you havent guitar:
	if (!m_spGuitar) {
		throw GuitarIsNotTurnedOn();
	}


	// Check known chords:
	std::string sound;
	if (chord == "Dmi") {
		sound = m_spGuitar->PlayChord(-1, -1, 0, 2, 3, 1);
	}
	else if (chord == "H7") {
		sound = m_spGuitar->PlayChord(-1, 2, 1, 2, 0, 2);
	}
	else {
		throw UnknownChord(chord);
	}

	// Format output:
	return chord + " sounds like '" + sound + "'";
}


void CPlayer::TurnGuitarOff() noexcept
{
	// Throw guitar away:
	m_spGuitar.reset();
}
