#include <iostream>
#include <sstream>
#include <memory>

class base
{
public:
	base() :
		m_name("base")
	{
	}

	void method()
	{
		std::cout << m_name << std::endl;
	}

private:
	std::string m_name;
};

class derived :
	public base
{
public:
	derived() :
		m_name("derived")
	{
	}

	void method()
	{
		std::cout << m_name << std::endl;
	}
private:
	std::string m_name;
};


class shape
{
public:
	shape() :
		m_name("shape")
	{
	}
	void method()
	{
		std::cout << m_name << std::endl;
	}

private:
	std::string m_name;

};

class indexable
{
public:
	indexable() :
		m_name("indexable")
	{
	}

	void method()
	{
		std::cout << m_name << std::endl;
	}

private:
	std::string m_name;
};

class circle :
	public shape,
	public indexable
{
public:
	void method()
	{
		std::cout << "circle" << std::endl;
	}

private:
	std::string m_name;
};

class standalone
{
public:
	standalone() :
		m_name("standalone")
	{
	}

	void method()
	{
		std::cout << m_name << std::endl;
	}

private:
	std::string m_name;
};

int main()
{
	// base -> derived
	{
		auto a = new base();
		auto b = new derived();

		base * c = dynamic_cast<derived *>(b);
		c->method();

		derived * d = static_cast<derived *>(c);
		d->method();

		delete a;
		delete b;
	}

	std::cout << std::endl;

	// shape, indexable -> circle
	{
		auto a = new circle();

		// upcast (could be static or dynamic)
		indexable * i = dynamic_cast<indexable *>(a);
		shape * s = dynamic_cast<shape *>(a);

		a->method();
		i->method();
		s->method();

		// downcast (only static and reinterpret; can't do typechecking on non-polymorphic class);
		circle * c;
		c = static_cast<circle *>(i);
		//c = dynamic_cast<circle *>(i);
		c = reinterpret_cast<circle *>(i);

		circle * d = static_cast<circle *>(s);

		c->method();
		d->method();

		// sidecast
		//shape * q = static_cast<shape *>(i);


		// illegal cast
		standalone * x = reinterpret_cast<standalone * >(c);
		//x->method();

		delete a;
	}

	std::cout << std::endl;

	// reference casting
	{
		base a;
		derived b;

		// upcast
		//base & c = static_cast<base &>(b);
		//base & c = b;
		base & c = dynamic_cast<base &>(b);
		c.method();

		// downcast
		derived & d = static_cast<derived &>(c);
		d.method();
	}

	std::getchar();

	return 0;
}