void not_modifying(const int * ptr)
{
	int local_int = 0;

	ptr = &local_int;
	//*ptr = 6;
}

void modifying(int * const ptr)
{
	int local_int = 0;

	//ptr = &local_int;
	*ptr = 6;
}

int main()
{
	int my_int = 2;

	modifying(&my_int);
	not_modifying(&my_int);

	return 0;
}