#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test:
#include "../lib/player.h"

// Fake implementation of interfaces:
#include "../lib/fakes/guitar_fake.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// There is tests for UTExample lib, so all classes in lib should be in this namespace.
namespace UTExample_test
{

	// This is test for one class:
	TEST_CLASS(CPlayer_test)
	{
	public:

		/*	This is class under test.
			You cannot simple use CPlayer because you need to inject fake implementation of guitar. */
		struct CPlayer_test_class:
			public CPlayer
		{
			CPlayer_test_class()
			{
				m_spGuitarFake = std::make_shared<CGuitar_fake>();
			}

			std::shared_ptr<IGuitar> CreateGuitar() const override
			{
				return m_spGuitarFake;
			}

			// Fake guitar implementation:
			std::shared_ptr<CGuitar_fake> m_spGuitarFake;
		};

		/*	This is SetUp for all tests in 'CPlayer_test'
			There is everything what is needed to be done before run test case. */
		CPlayer_test()
		{
			m_spPlayer = std::make_shared<CPlayer_test_class>();
		}

		/*	This is TearDown for all tests in 'CPlayer_test'
			There is everything what is needed to be done after run test case. */
		~CPlayer_test()
		{
			m_spPlayer.reset();
		}

		// Instantion of tested class:
		std::shared_ptr<CPlayer_test_class> m_spPlayer;


		/*			THERE BEGIN UNIT TEST		*/

		// Happy scenarios:
		TEST_METHOD(PlayDmi)
		{
			// Prepare:
			// - You need to prepare outputs from guitar class:
			m_spPlayer->m_spGuitarFake->m_rv = "tatata";

			// Test:
			m_spPlayer->TurnGuitarOn();
			std::string ret = m_spPlayer->PlayChord("Dmi");

			// Verify:
			Assert::AreEqual(ret.c_str(), "Dmi sounds like 'tatata'");
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sE, -1);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sA, -1);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sD, 0);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sG, 2);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sH, 3);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_se, 1);
		}

		TEST_METHOD(PlayH7)
		{
			// Prepare:
			m_spPlayer->m_spGuitarFake->m_rv = "tututu";

			// Test:
			m_spPlayer->TurnGuitarOn();
			std::string ret = m_spPlayer->PlayChord("H7");

			// Verify:
			Assert::AreEqual(ret.c_str(), "H7 sounds like 'tututu'");
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sE, -1);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sA, 2);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sD, 1);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sG, 2);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_sH, 0);
			Assert::AreEqual(m_spPlayer->m_spGuitarFake->m_se, 2);
		}

		// Wrong usage:
		TEST_METHOD(UnknownChord)
		{
			// Prepare:
			bool exceptionThrown = false;

			// Test:
			m_spPlayer->TurnGuitarOn();
			try {
				m_spPlayer->PlayChord("CMaj7");
			}
			catch (const IPlayer::UnknownChord&) {
				exceptionThrown = true;
			}

			// Verify:
			Assert::IsFalse(m_spPlayer->m_spGuitarFake->m_wasCalled);
			Assert::IsTrue(exceptionThrown);
		}

		TEST_METHOD(GuitarIsNotTurnedOn)
		{
			// Prepare:
			bool exceptionThrown = false;

			// Test:
			try {
				m_spPlayer->PlayChord("Dmi");
			}
			catch (const IPlayer::GuitarIsNotTurnedOn&) {
				exceptionThrown = true;
			}

			// Verify:
			Assert::IsFalse(m_spPlayer->m_spGuitarFake->m_wasCalled);
			Assert::IsTrue(exceptionThrown);
		}

		TEST_METHOD(PlayAfterTurningOff)
		{
			// Prepare:
			bool exceptionThrown = false;

			// Test:
			m_spPlayer->TurnGuitarOn();
			m_spPlayer->PlayChord("Dmi");
			m_spPlayer->TurnGuitarOff();

			try {
				m_spPlayer->PlayChord("Dmi");
			}
			catch (const IPlayer::GuitarIsNotTurnedOn&) {
				exceptionThrown = true;
			}

			// Verify:
			Assert::IsTrue(exceptionThrown);
		}

		TEST_METHOD(PlayChordFailed_TearString)
		{
			// Prepare:
			bool exceptionThrown = false;
			m_spPlayer->m_spGuitarFake->m_tearString = 2;

			// Test:
			m_spPlayer->TurnGuitarOn();

			try {
				m_spPlayer->PlayChord("Dmi");
			}
			catch (const IGuitar::TearString& e) {
				exceptionThrown = true;
				IGuitar::TearString exc(2);
				Assert::AreEqual(exc.what(), e.what());
			}

			// Verify:
			Assert::IsTrue(exceptionThrown);
		}
	};

	// This is test for other class:
	TEST_CLASS(CGuitar_test)
	{
		TEST_METHOD(PlayChord)
		{
			CGuitar g;
			std::string sound = g.PlayChord(1, 2, 3, 4, 5, 6);
			Assert::AreEqual(sound.c_str(), "Nice chord");
		}
	};
}