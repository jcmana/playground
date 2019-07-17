// Parameter pack:
// http://en.cppreference.com/w/cpp/language/parameter_pack

template<typename T_impl, typename ...T_param_pack>
void template_schedule(T_impl impl, T_param_pack... params)
{
	impl(params...);
}

void callback_impl(int a, int b)
{
	(void)(a);
	(void)(b);
}

int main()
{
	template_schedule(callback_impl, 1, 2);

	return 0;
}