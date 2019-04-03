#include "../precision_scalar/precision_scalar.hpp"
#include "../precision_scalar/fixed.hpp"

int main()
{
	if (false)
	{
		precision_scalar<6> a(7ll);
		precision_scalar<4> b(3ll);

		//auto eq = a == b;
		//auto gt = a > b;

		//auto add_a_b = add(a, b);

		//auto sub_a_b = sub(a, b);
		//auto sub_b_a = sub(b, a);

		auto ll = static_cast<long long int>(a);
		auto d = static_cast<double>(b);
	}

	fixed a(176.3);
	fixed b(10.18687);
	double add_ab = add(a, b);
	double sub_ba = sub(b, a);

	long long mul_ab_ll = mul(a, b);
	double mul_ab_d = mul(a, b);

	return 0;
}