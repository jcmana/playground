#include "A.h"
#include "B.h"

int main()
{
	A a(7);
	B b(14);

	A aa = static_cast<A>(b);	// explicit conversion (explicit operator A() const)
	B bb = a;		// implicit conversion (operator B() const)

	return 0;
}
