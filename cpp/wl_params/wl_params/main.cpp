#include <windows.h>

void picus(WPARAM wParam, LPARAM lParam)
{
	(*(int *)(wParam)) = 5;
}

int main()
{
	int hovno = 0;
	int sracka = 0;

	picus((WPARAM)(&hovno), (LPARAM)(&sracka));

	return 0;
}
