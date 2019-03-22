#include "A.h"
#include "B.h"

int main()
{
	TestA();
	TestB();

	// Produces error: LNK2038 mismatch detected for '_ITERATOR_DEBUG_LEVEL'

	return 0;
}
