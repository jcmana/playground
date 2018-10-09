#include <utility>

struct base
{
	enum { Type = 1 };

	base(int t) :
		m_type(t)
	{
	}

	int type() const
	{
		return m_type;
	}

	const int m_type;
};

struct derived_a :
	public base
{
	enum { Type = 2 };

	derived_a() :
		base(Type)
	{
	}
};

struct derived_b :
	public base
{
	enum { Type = 3 };

	derived_b() :
		base(Type)
	{
	}
};

template<typename T> inline T & auto_cast(base & b)
{
	switch (b.type)
	{
		case 0: return static_cast<derived_a &>(b);
		case 1: return static_cast<derived_b &>(b);
	}
}

template <class T> inline T qt_auto_cast(base * item)
{
	typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type Item;
	return (int(Item::Type) == int(base::Type) || (item && int(Item::Type) == item->type())) ? static_cast<T>(item) : 0;
}


int main()
{
	derived_a a;

	base & ptr = a;

	derived_a * x = qt_auto_cast(&a);

	return 0;
}