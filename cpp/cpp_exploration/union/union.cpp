#include <vector>
#include <string>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

struct composite
{
	composite() :
		type(0),
		a{1, 2, 3}
	{
	}
	~composite()
	{
		//using namespace std;
		
		if (type == 0)
		{
			a.~vector();
			return;
		}

		if (type == 1)
		{
			b.~basic_string();
			return;
		}
	}

	unsigned short int type;

	union 
	{
		std::vector<int> a;
		std::string b;
	};

	// If union contains non-POD types (as in types that have constructors and destructors)
	// it requires constructor and destructor explicit definition.
};

int main()
{
	{
		composite c;
	}

	_CrtDumpMemoryLeaks(); return 0;
}
